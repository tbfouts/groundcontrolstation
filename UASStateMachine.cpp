#include "UASStateMachine.hpp"

UASStateMachine::UASStateMachine(QObject* parent)
    : QObject(parent)
    , m_currentState(UASState::Landed)
{
}

UASStateMachine::~UASStateMachine()
{
}

UASState::State UASStateMachine::currentState() const
{
    return m_currentState;
}

void UASStateMachine::takeOff()
{
    if (m_currentState == UASState::Landed) {
        setCurrentState(UASState::TakingOff);
        qDebug() << "Taking off...";
    } else {
        qDebug() << "Cannot take off: UAS is not in landed state";
    }
}

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

void UASStateMachine::loiter()
{
    if (m_currentState == UASState::Flying || m_currentState == UASState::FlyingToWaypoint) {
        setCurrentState(UASState::Loitering);
        qDebug() << "Loitering...";
    } else {
        qDebug() << "Cannot loiter: UAS is not in a flying state";
    }
}

void UASStateMachine::fly()
{
    if (m_currentState == UASState::Loitering || m_currentState == UASState::TakingOff) {
        setCurrentState(UASState::Flying);
        qDebug() << "Flying...";
    } else {
        qDebug() << "Cannot fly: UAS is not in loitering or taking off state";
    }
}

void UASStateMachine::setCurrentState(UASState::State state)
{
    if (m_currentState != state) {
        m_currentState = state;
        qDebug() << "UAS State changed to:" << state;
        emit currentStateChanged(m_currentState);
    }
}
