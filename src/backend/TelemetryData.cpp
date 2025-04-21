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
    m_stateMachine->takeOff();
}

/**
 * @brief Commands the UAS to land
 * 
 * Delegates the landing command to the UAS state machine.
 * This command is ignored if the UAS is not in a flying state.
 */
void TelemetryData::land()
{
    m_stateMachine->land();
}

/**
 * @brief Commands the UAS to fly
 * 
 * Delegates the fly command to the UAS state machine.
 * This command is typically used to transition from loitering to normal flight.
 */
void TelemetryData::fly()
{
    m_stateMachine->fly();
}
