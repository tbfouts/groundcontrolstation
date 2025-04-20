#ifndef TELEMETRYDATA_HPP
#define TELEMETRYDATA_HPP

#include <QObject>
#include <QGeoCoordinate>
#include <QtQml/qqmlregistration.h>
#include "UASStateMachine.hpp"

class TelemetryData : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(int battery READ battery NOTIFY batteryChanged)
    Q_PROPERTY(int altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(QGeoCoordinate position READ position NOTIFY positionChanged)
    Q_PROPERTY(UASState::State state READ state NOTIFY stateChanged)
    
public:
    explicit TelemetryData(QObject* parent = nullptr);
    virtual ~TelemetryData();
    
    virtual int battery() const = 0;
    virtual int altitude() const = 0;
    virtual int speed() const = 0;
    virtual QGeoCoordinate position() const = 0;
    
    // State access
    UASState::State state() const;
    
    // State control commands
    Q_INVOKABLE void takeOff();
    Q_INVOKABLE void land();
    Q_INVOKABLE void fly();
    
    // Navigation commands
    Q_INVOKABLE virtual void goTo(const QGeoCoordinate& destination) = 0;
    
signals:
    void batteryChanged(int battery);
    void altitudeChanged(int altitude);
    void speedChanged(int speed);
    void positionChanged(QGeoCoordinate position);
    void stateChanged(UASState::State state);
    
protected:
    UASStateMachine* m_stateMachine;
};

#endif // TELEMETRYDATA_HPP
