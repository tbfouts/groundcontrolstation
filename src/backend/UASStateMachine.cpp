#include "UASStateMachine.hpp"

/**
 * @brief Constructs the UASStateMachine with default state Landed
 * @param parent The parent QObject
 */
UASStateMachine::UASStateMachine(QObject* parent)
    : QObject(parent)
    , m_currentState(UASState::Landed)
{
}

/**
 * @brief Destructor
 */
UASStateMachine::~UASStateMachine()
{
}

/**
 * @brief Gets the current state of the UAS
 * @return The current state
 */
UASState::State UASStateMachine::currentState() const
{
    return m_currentState;
}

/**
 * @brief Directly sets the current state of the UAS
 * @param state The new state to set
 * @return true If the state change was successful, false otherwise.
 * 
 * Updates the state if it's different from the current state,
 * logs the change, and emits the currentStateChanged signal.
 */
bool UASStateMachine::setCurrentState(UASState::State state)
{
    bool acceptStateChange = true;

    if (m_currentState != state)
    {
        switch (state) {
        case UASState::Landing:
            if (UASState::Flying != m_currentState &&
                UASState::FlyingToWaypoint != m_currentState &&
                UASState::Loitering != m_currentState)
            {
                qWarning() << "Cannot land unless in a valid flying state";
                acceptStateChange = false;
            }
            break;
        case UASState::TakingOff:
            if (UASState::Landed != m_currentState)
            {
                qWarning() << "Cannot takeoff unless in a landed state";
                acceptStateChange = false;
            }
            break;
        case UASState::FlyingToWaypoint:
            if (UASState::Flying != m_currentState &&
                UASState::FlyingToWaypoint != m_currentState &&
                UASState::Loitering != m_currentState)
            {
                qWarning() << "Cannot fly to waypoint unless already in a valid flying state";
                acceptStateChange = false;
            }
            break;
        default:
            // all other states intentionally accepted
            break;
        }
    }

    if (acceptStateChange)
    {
        m_currentState = state;
        qDebug() << "UAS State changed to:" << state;
        emit currentStateChanged(m_currentState);
    }

    return acceptStateChange;
}
