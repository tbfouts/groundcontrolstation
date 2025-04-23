#include <QtTest/QTest>
#include <QSignalSpy>
#include <QGeoCoordinate>
#include <QObject>
#include <QVariant>
#include <QDebug>
#include "TelemetryDataSimulator.hpp"

class TestTelemetryDataSimulator : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testGoTo();
    void testInvalidStateTransitions();
    void cleanupTestCase();

private:
    UASStateMachine* m_stateMachine;
    TelemetryDataSimulator* m_simulator;
};

void TestTelemetryDataSimulator::initTestCase()
{
    // Create a new UASStateMachine that we control
    m_stateMachine = new UASStateMachine(this);
    QVERIFY(m_stateMachine != nullptr);
    
    // Create a new TelemetryDataSimulator with our state machine
    m_simulator = new TelemetryDataSimulator(m_stateMachine);
    QVERIFY(m_simulator != nullptr);
}

void TestTelemetryDataSimulator::testGoTo()
{
    // Set up spy for state changes
    QSignalSpy stateSpy(m_stateMachine, &UASStateMachine::currentStateChanged);
    QVERIFY(stateSpy.isValid());
    
    // Set up spy for position changes
    QSignalSpy positionSpy(m_simulator, &TelemetryDataSimulator::positionChanged);
    QVERIFY(positionSpy.isValid());
    
    // Put UAS in flying state
    m_stateMachine->setCurrentState(UASState::Flying);
    stateSpy.clear();
    
    // Initial position
    QGeoCoordinate initialPosition = m_simulator->position();
    
    // Create a destination coordinate (500m away)
    QGeoCoordinate destination(
        initialPosition.latitude() + 0.005,  // approx 500m north
        initialPosition.longitude() + 0.005  // approx 500m east
    );
    
    // Call goTo with loiter parameters
    const int loiterRadius = 100;
    const bool loiterClockwise = true;
    m_simulator->goTo(destination, loiterRadius, loiterClockwise);
    
    // Verify state changed to FlyingToWaypoint
    QVERIFY(stateSpy.wait(100)); // Wait for signal
    QCOMPARE(stateSpy.takeFirst().at(0).value<UASState::State>(), UASState::FlyingToWaypoint);
    
    // Verify position is changing (should emit at least one position change)
    QVERIFY(positionSpy.wait(500)); // Wait up to 500ms for first position update
    QVERIFY(positionSpy.count() > 0);
}

void TestTelemetryDataSimulator::testInvalidStateTransitions()
{
    // Test invalid state transitions to ensure they're properly rejected
    
    // Case 1: Cannot take off when already flying
    m_stateMachine->setCurrentState(UASState::Flying);
    
    // Attempt to take off while already flying
    m_simulator->takeOff();
    
    // The state should remain Flying (takeoff should be rejected)
    QCOMPARE(m_stateMachine->currentState(), UASState::Flying);
    
    // Case 2: Cannot land when already on the ground
    m_stateMachine->setCurrentState(UASState::Landed);
    
    // Attempt to land while already on the ground
    m_simulator->land();
    
    // The state should remain Landed (landing should be rejected)
    QCOMPARE(m_stateMachine->currentState(), UASState::Landed);
    
    // Case 3: Cannot go to waypoint when not in a flying state
    m_stateMachine->setCurrentState(UASState::Landed);
    
    // Initial position
    QGeoCoordinate initialPosition = m_simulator->position();
    
    // Create a destination coordinate
    QGeoCoordinate destination(
        initialPosition.latitude() + 0.005,
        initialPosition.longitude() + 0.005
    );
    
    // Attempt to go to waypoint while landed
    m_simulator->goTo(destination, 100, true);
    
    // The state should remain Landed (goTo should be rejected)
    QCOMPARE(m_stateMachine->currentState(), UASState::Landed);
}

void TestTelemetryDataSimulator::cleanupTestCase()
{
    // Clean up the test fixture
    delete m_simulator;
    m_simulator = nullptr;
    
    // State machine will be deleted by QObject parent (this)
}

// Using QTest's own QTEST_MAIN macro
QTEST_MAIN(TestTelemetryDataSimulator)
#include "TestTelemetryDataSimulator.moc"
