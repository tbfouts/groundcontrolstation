#ifndef UNCREWEDAERIALSYSTEM_HPP
#define UNCREWEDAERIALSYSTEM_HPP

#include <QObject>
#include <QSharedPointer>
#include "TelemetryData.hpp"

class UncrewedAerialSystem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TelemetryData* telemetry READ telemetry CONSTANT)
    
public:
    explicit UncrewedAerialSystem(QSharedPointer<TelemetryData> telemetryData, QObject* parent = nullptr);
    virtual ~UncrewedAerialSystem();
    
    TelemetryData* telemetry() const;
    
    static void registerQmlType();
    
private:
    QSharedPointer<TelemetryData> m_telemetryData;
};

#endif // UNCREWEDAERIALSYSTEM_HPP