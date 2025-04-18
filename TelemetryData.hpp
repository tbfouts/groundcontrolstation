#ifndef TELEMETRYDATA_HPP
#define TELEMETRYDATA_HPP

#include <QObject>
#include <QGeoCoordinate>

class TelemetryData : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(int battery READ battery NOTIFY batteryChanged)
    Q_PROPERTY(int altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(QGeoCoordinate position READ position NOTIFY positionChanged)
    
public:
    explicit TelemetryData(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~TelemetryData() = default;
    
    virtual int battery() const = 0;
    virtual int altitude() const = 0;
    virtual int speed() const = 0;
    virtual QGeoCoordinate position() const = 0;
    
signals:
    void batteryChanged(int battery);
    void altitudeChanged(int altitude);
    void speedChanged(int speed);
    void positionChanged(QGeoCoordinate position);
};

#endif // TELEMETRYDATA_HPP