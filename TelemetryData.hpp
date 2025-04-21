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
    Q_PROPERTY(int loiterRadius READ loiterRadius WRITE setLoiterRadius NOTIFY loiterRadiusChanged)
    Q_PROPERTY(bool loiterClockwise READ loiterClockwise WRITE setLoiterClockwise NOTIFY loiterClockwiseChanged)

public:
    explicit TelemetryData(QObject* parent = nullptr);
    virtual ~TelemetryData();

    virtual int battery() const = 0;
    virtual int altitude() const = 0;
    virtual int speed() const = 0;
    virtual QGeoCoordinate position() const = 0;

    // State access
    UASState::State state() const;

    // Loiter properties access
    virtual int loiterRadius() const = 0;
    virtual void setLoiterRadius(int radius) = 0;

    virtual bool loiterClockwise() const = 0;
    virtual void setLoiterClockwise(bool clockwise) = 0;

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
    void loiterRadiusChanged(int radius);
    void loiterClockwiseChanged(bool clockwise);

protected:
    UASStateMachine* m_stateMachine;
};

#endif // TELEMETRYDATA_HPP
