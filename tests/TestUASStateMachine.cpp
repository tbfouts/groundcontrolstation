#include <QtTest/QTest>
#include <QSignalSpy>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QDebug>
#include "UASStateMachine.hpp"

class TestUASStateMachine : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testInitialState();
    void testStateTransitions();
    void testSignalEmission();
    void testCommandSignals();
    void testInvalidTransitions();
    void testStateEnum();
    void cleanupTestCase();

private:
    UASStateMachine* m_stateMachine;
    
    // Helper function to verify a state change signal
    void verifyStateChangeSignal(QSignalSpy& spy, UASState::State expectedState);
};

void TestUASStateMachine::initTestCase()
{
    // Create a new UASStateMachine instance for testing
    m_stateMachine = new UASStateMachine();
    QVERIFY(m_stateMachine != nullptr);
}

void TestUASStateMachine::testInitialState()
{
    // Verify the initial state is Landed
    QCOMPARE(m_stateMachine->currentState(), UASState::Landed);
}

void TestUASStateMachine::testStateTransitions()
{
    // Set up spy to monitor state changes
    QSignalSpy spy(m_stateMachine, &UASStateMachine::currentStateChanged);
    QVERIFY(spy.isValid());
    
    // ==== Test typical flight sequence with transitions ====
    // Set to Landed as starting point
    m_stateMachine->setCurrentState(UASState::Landed);
    spy.clear();
    
    // 1. Landed -> TakingOff (via takeOff command)
    m_stateMachine->takeOff();
    QCOMPARE(m_stateMachine->currentState(), UASState::TakingOff);
    verifyStateChangeSignal(spy, UASState::TakingOff);
    
    // 2. TakingOff -> Flying (via setCurrentState)
    spy.clear();
    m_stateMachine->setCurrentState(UASState::Flying);
    QCOMPARE(m_stateMachine->currentState(), UASState::Flying);
    verifyStateChangeSignal(spy, UASState::Flying);
    
    // 3. Flying -> FlyingToWaypoint (via setCurrentState)
    spy.clear();
    m_stateMachine->setCurrentState(UASState::FlyingToWaypoint);
    QCOMPARE(m_stateMachine->currentState(), UASState::FlyingToWaypoint);
    verifyStateChangeSignal(spy, UASState::FlyingToWaypoint);
    
    // 4. FlyingToWaypoint -> Loitering (via loiter command)
    spy.clear();
    m_stateMachine->loiter();
    QCOMPARE(m_stateMachine->currentState(), UASState::Loitering);
    verifyStateChangeSignal(spy, UASState::Loitering);
    
    // 5. Loitering -> Flying (via fly command)
    spy.clear();
    m_stateMachine->fly();
    QCOMPARE(m_stateMachine->currentState(), UASState::Flying);
    verifyStateChangeSignal(spy, UASState::Flying);
    
    // 6. Flying -> Landing (via land command)
    spy.clear();
    m_stateMachine->land();
    QCOMPARE(m_stateMachine->currentState(), UASState::Landing);
    verifyStateChangeSignal(spy, UASState::Landing);
    
    // 7. Landing -> Landed (via setCurrentState)
    spy.clear();
    m_stateMachine->setCurrentState(UASState::Landed);
    QCOMPARE(m_stateMachine->currentState(), UASState::Landed);
    verifyStateChangeSignal(spy, UASState::Landed);
    
    // All transitions completed and verified
}

// Helper function to verify a state change signal was emitted with the correct state
void TestUASStateMachine::verifyStateChangeSignal(QSignalSpy& spy, UASState::State expectedState)
{
    // Verify at least one signal was emitted
    QVERIFY2(spy.count() >= 1, QString("No signal emitted, expected state: %1").arg(expectedState).toUtf8());
    
    // Get the last emitted signal's arguments
    const QList<QVariant>& arguments = spy.takeLast();
    
    // Verify the signal has exactly one argument (the state)
    QCOMPARE(arguments.count(), 1);
    
    // Verify the emitted state matches the expected state
    const UASState::State actualState = arguments.at(0).value<UASState::State>();
    QCOMPARE(actualState, expectedState);
}

void TestUASStateMachine::testSignalEmission()
{
    // Set up signal spy to capture state change signals
    QSignalSpy spy(m_stateMachine, &UASStateMachine::currentStateChanged);
    QVERIFY(spy.isValid());
    
    // Verify signal count starts at 0
    QCOMPARE(spy.count(), 0);
    
    // Start with a known state
    m_stateMachine->setCurrentState(UASState::Landed);
    spy.clear(); // Clear any signals from the setup
    
    // Verify fast multiple state changes emit the correct signals
    m_stateMachine->setCurrentState(UASState::TakingOff);
    m_stateMachine->setCurrentState(UASState::Flying);
    m_stateMachine->setCurrentState(UASState::Landing);
    
    // Verify we got exactly 3 signals
    QCOMPARE(spy.count(), 3);
    
    // Verify signals in order
    QList<QVariant> firstSignal = spy.at(0);
    QList<QVariant> secondSignal = spy.at(1);
    QList<QVariant> thirdSignal = spy.at(2);
    
    QCOMPARE(firstSignal.at(0).value<UASState::State>(), UASState::TakingOff);
    QCOMPARE(secondSignal.at(0).value<UASState::State>(), UASState::Flying);
    QCOMPARE(thirdSignal.at(0).value<UASState::State>(), UASState::Landing);
    
    // Test that setting the same state doesn't emit a signal
    spy.clear();
    m_stateMachine->setCurrentState(UASState::Landing); // Already in this state
    QCOMPARE(spy.count(), 0);
    
    // Test with wait to ensure all signals are processed
    spy.clear();
    m_stateMachine->setCurrentState(UASState::Landed);
    QVERIFY(spy.wait(100)); // Wait up to 100ms for signals
    QCOMPARE(spy.count(), 1);
    verifyStateChangeSignal(spy, UASState::Landed);
}

void TestUASStateMachine::testCommandSignals()
{
    // Test that commands emit the appropriate signals
    
    // Setup: Reset to landed state
    m_stateMachine->setCurrentState(UASState::Landed);
    
    // Test takeOff() command
    QSignalSpy takeoffSpy(m_stateMachine, &UASStateMachine::currentStateChanged);
    QVERIFY(takeoffSpy.isValid());
    
    // Execute takeOff command and verify
    m_stateMachine->takeOff();
    QVERIFY(takeoffSpy.wait(100)); // Wait for signal
    QCOMPARE(takeoffSpy.count(), 1);
    QCOMPARE(takeoffSpy.at(0).at(0).value<UASState::State>(), UASState::TakingOff);
    
    // Test state is in Flying
    m_stateMachine->setCurrentState(UASState::Flying);
    takeoffSpy.clear();
    
    // Test loiter() command
    m_stateMachine->loiter();
    QVERIFY(takeoffSpy.wait(100)); // Wait for signal
    QCOMPARE(takeoffSpy.count(), 1);
    QCOMPARE(takeoffSpy.at(0).at(0).value<UASState::State>(), UASState::Loitering);
    
    // Test fly() command - go back to flying
    takeoffSpy.clear();
    m_stateMachine->fly();
    QVERIFY(takeoffSpy.wait(100)); // Wait for signal
    QCOMPARE(takeoffSpy.count(), 1);
    QCOMPARE(takeoffSpy.at(0).at(0).value<UASState::State>(), UASState::Flying);
    
    // Test land() command
    takeoffSpy.clear();
    m_stateMachine->land();
    QVERIFY(takeoffSpy.wait(100)); // Wait for signal
    QCOMPARE(takeoffSpy.count(), 1);
    QCOMPARE(takeoffSpy.at(0).at(0).value<UASState::State>(), UASState::Landing);
}

void TestUASStateMachine::testInvalidTransitions()
{
    // If the UASStateMachine enforces state transition rules,
    // this test should verify that invalid transitions are rejected.
    // Currently, the UASStateMachine class allows any transition, so
    // this test is a placeholder for future validation.
    
    // Example: Can't take off if already flying
    m_stateMachine->setCurrentState(UASState::Flying);
    QSignalSpy spy(m_stateMachine, &UASStateMachine::currentStateChanged);
    
    // Reset spy for clean test
    spy.clear();
    
    // Attempt invalid operation - try to take off when already flying
    // Note: The current implementation might allow this, but a robust
    // state machine should prevent it
    m_stateMachine->takeOff();
    
    // Check if the takeoff command was ignored or handled
    // This assertion should be adjusted based on how invalid transitions are handled
    // in the actual implementation
    QCOMPARE(m_stateMachine->currentState(), UASState::Flying);
    
    // Check if moving from landed to flying directly works
    // (should require TakingOff state in between)
    m_stateMachine->setCurrentState(UASState::Landed);
    spy.clear();
    
    // Try to set to Flying directly from Landed
    m_stateMachine->setCurrentState(UASState::Flying);
    
    // Again, this is a placeholder - adjust based on implementation rules
    QCOMPARE(m_stateMachine->currentState(), UASState::Flying);
}

void TestUASStateMachine::testStateEnum()
{
    // Test enum values match expected order
    QCOMPARE(static_cast<int>(UASState::Landed), 0);
    QCOMPARE(static_cast<int>(UASState::TakingOff), 1);
    QCOMPARE(static_cast<int>(UASState::Flying), 2);
    QCOMPARE(static_cast<int>(UASState::FlyingToWaypoint), 3);
    QCOMPARE(static_cast<int>(UASState::Loitering), 4);
    QCOMPARE(static_cast<int>(UASState::Landing), 5);
    
    // Verify enum value boundaries
    QVERIFY(static_cast<int>(UASState::Landed) >= 0);
    QVERIFY(static_cast<int>(UASState::Landing) <= 5);
    
    // Test meta-enum introspection when QMetaEnum is available
    // This is useful for verifying the enum is properly registered with Qt's meta-object system
    const QMetaObject *metaObject = &UASState::staticMetaObject;
    int enumIndex = metaObject->indexOfEnumerator("State");
    QVERIFY(enumIndex != -1);
    
    QMetaEnum metaEnum = metaObject->enumerator(enumIndex);
    QCOMPARE(metaEnum.keyCount(), 6); // Total number of enum values
    
    // Verify key names match
    QCOMPARE(QString(metaEnum.key(0)), QString("Landed"));
    QCOMPARE(QString(metaEnum.key(1)), QString("TakingOff"));
    QCOMPARE(QString(metaEnum.key(2)), QString("Flying"));
    QCOMPARE(QString(metaEnum.key(3)), QString("FlyingToWaypoint"));
    QCOMPARE(QString(metaEnum.key(4)), QString("Loitering"));
    QCOMPARE(QString(metaEnum.key(5)), QString("Landing"));
}

void TestUASStateMachine::cleanupTestCase()
{
    // Clean up the test fixture
    delete m_stateMachine;
    m_stateMachine = nullptr;
}

// Using QTest's own QTEST_MAIN macro
QTEST_MAIN(TestUASStateMachine)
#include "TestUASStateMachine.moc"
