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
 * @brief Commands the UAS to take off
 * 
 * Transitions from Landed to TakingOff state if currently landed.
 * Logs a debug message if the transition is not possible.
 */
void UASStateMachine::takeOff()
{
    if (m_currentState == UASState::Landed) {
        setCurrentState(UASState::TakingOff);
        qDebug() << "Taking off...";
    } else {
        qDebug() << "Cannot take off: UAS is not in landed state";
    }
}

/**
 * @brief Commands the UAS to land
 * 
 * Transitions to Landing state if currently in a flyable state
 * (Flying, FlyingToWaypoint, or Loitering).
 * Logs a debug message if the transition is not possible.
 */
void UASStateMachine::land()
{
    if (m_currentState == UASState::Flying || 
        m_currentState == UASState::FlyingToWaypoint || 
        m_currentState == UASState::Loitering) {
        setCurrentState(UASState::Landing);
        qDebug() << "Landing...";
    } else {
        qDebug() << "Cannot land: UAS is not in a flyable state";
    }
}

/**
 * @brief Commands the UAS to loiter around the current position
 * 
 * Transitions to Loitering state if currently in a flying state
 * (Flying or FlyingToWaypoint).
 * Logs a debug message if the transition is not possible.
 */
void UASStateMachine::loiter()
{
    if (m_currentState == UASState::Flying || m_currentState == UASState::FlyingToWaypoint) {
        setCurrentState(UASState::Loitering);
        qDebug() << "Loitering...";
    } else {
        qDebug() << "Cannot loiter: UAS is not in a flying state";
    }
}

/**
 * @brief Commands the UAS to fly normally
 * 
 * Transitions to Flying state if currently Loitering or TakingOff.
 * Logs a debug message if the transition is not possible.
 */
void UASStateMachine::fly()
{
    if (m_currentState == UASState::Loitering || m_currentState == UASState::TakingOff) {
        setCurrentState(UASState::Flying);
        qDebug() << "Flying...";
    } else {
        qDebug() << "Cannot fly: UAS is not in loitering or taking off state";
    }
}

/**
 * @brief Directly sets the current state of the UAS
 * @param state The new state to set
 * 
 * Updates the state if it's different from the current state,
 * logs the change, and emits the currentStateChanged signal.
 */
void UASStateMachine::setCurrentState(UASState::State state)
{
    if (m_currentState != state) {
        m_currentState = state;
        qDebug() << "UAS State changed to:" << state;
        emit currentStateChanged(m_currentState);
    }
}
