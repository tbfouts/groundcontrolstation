#include "TelemetryData.hpp"

/**
 * @brief Constructs a TelemetryData object and creates the state machine
 * @param parent The parent QObject
 * 
 * Creates a new UAS state machine and connects its state change signal to
 * the TelemetryData class's stateChanged signal.
 */
TelemetryData::TelemetryData(QObject *parent)
    : QObject(parent)
    , m_stateMachine(new UASStateMachine(this))
{
    // Connect state machine signals
    connect(m_stateMachine, &UASStateMachine::currentStateChanged,
            this, &TelemetryData::stateChanged);
}

/**
 * @brief Constructs a TelemetryData object with a provided state machine
 * @param stateMachine External state machine to use
 * @param parent The parent QObject
 *
 * Uses the provided state machine instead of creating a new one.
 * This is useful for testing to provide a mock or controlled state machine.
 */
TelemetryData::TelemetryData(UASStateMachine* stateMachine, QObject *parent)
    : QObject(parent)
    , m_stateMachine(stateMachine)
{
    // Connect state machine signals
    connect(m_stateMachine, &UASStateMachine::currentStateChanged,
            this, &TelemetryData::stateChanged);
}

/**
 * @brief Destructor
 */
TelemetryData::~TelemetryData()
{
}

/**
 * @brief Gets the current UAS state
 * @return The current state of the UAS state machine
 */
UASState::State TelemetryData::state() const
{
    return m_stateMachine->currentState();
}

/**
 * @brief Commands the UAS to take off
 * 
 * Delegates the takeoff command to the UAS state machine.
 * This command is ignored if the UAS is not in the landed state.
 */
void TelemetryData::takeOff()
{
    m_stateMachine->setCurrentState(UASState::TakingOff);
}

/**
 * @brief Commands the UAS to land
 * 
 * Delegates the landing command to the UAS state machine.
 * This command is ignored if the UAS is not in a flying state.
 */
void TelemetryData::land()
{
    m_stateMachine->setCurrentState(UASState::Landing);
}

void TelemetryData::goTo(const QGeoCoordinate& destination, const int loiterRadius, const bool loiterClockwise)
{
    m_stateMachine->setCurrentState(UASState::FlyingToWaypoint);
}
