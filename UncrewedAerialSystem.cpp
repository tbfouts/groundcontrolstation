#include "UncrewedAerialSystem.hpp"
#include "TelemetryDataSimulator.hpp"
#include <QQmlEngine>
#include <QtQml>

UncrewedAerialSystem::UncrewedAerialSystem(QSharedPointer<TelemetryData> telemetryData, QObject* parent)
    : QObject(parent)
    , m_telemetryData(telemetryData)
{
    // If the TelemetryData instance is created specifically for this UAS, set parent
    if (m_telemetryData && m_telemetryData->parent() == nullptr) {
        m_telemetryData->setParent(this);
    }
}

UncrewedAerialSystem::~UncrewedAerialSystem()
{
}

TelemetryData* UncrewedAerialSystem::telemetry() const
{
    return m_telemetryData.data();
}

void UncrewedAerialSystem::registerQmlType()
{
    // Create a TelemetryDataSimulator
    QSharedPointer<TelemetryData> telemetryData = QSharedPointer<TelemetryDataSimulator>::create();
    
    // Cast to the simulator implementation to start updates
    auto simulator = telemetryData.dynamicCast<TelemetryDataSimulator>();
    if (simulator) {
        simulator->start();
    }
    
    // Create the UncrewedAerialSystem singleton instance
    static auto* instance = new UncrewedAerialSystem(telemetryData);
    
    // Register the singleton instance with QML
    qmlRegisterSingletonInstance<UncrewedAerialSystem>("GroundControlStation", 1, 0, "UncrewedAerialSystem", instance);
}