#include "TelemetryData.hpp"

TelemetryData::TelemetryData(QObject *parent)
    : QObject(parent)
    , m_stateMachine(new UASStateMachine(this))
{
    // Connect state machine signals
    connect(m_stateMachine, &UASStateMachine::currentStateChanged,
            this, &TelemetryData::stateChanged);
}

TelemetryData::~TelemetryData()
{
}

UASState::State TelemetryData::state() const
{
    return m_stateMachine->currentState();
}

void TelemetryData::takeOff()
{
    m_stateMachine->takeOff();
}

void TelemetryData::land()
{
    m_stateMachine->land();
}

void TelemetryData::fly()
{
    m_stateMachine->fly();
}
