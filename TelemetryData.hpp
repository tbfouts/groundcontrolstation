#ifndef TELEMETRYDATA_HPP
#define TELEMETRYDATA_HPP

#include <QObject>
#include <QGeoCoordinate>
#include <QtQml/qqmlregistration.h>
#include "UASStateMachine.hpp"

/**
 * @class TelemetryData
 * @brief Base class for UAS telemetry data handling
 *
 * This abstract class defines the interface for accessing and controlling
 * the UAS telemetry data. It serves as a base class for concrete implementations
 * like TelemetryDataSimulator.
 */
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
    /**
     * @brief Constructs a TelemetryData object
     * @param parent The parent QObject
     */
    explicit TelemetryData(QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    virtual ~TelemetryData();

    /**
     * @brief Gets the current battery percentage
     * @return Battery level as a percentage (0-100)
     */
    virtual int battery() const = 0;
    
    /**
     * @brief Gets the current altitude
     * @return Altitude in meters
     */
    virtual int altitude() const = 0;
    
    /**
     * @brief Gets the current speed
     * @return Speed in meters per second
     */
    virtual int speed() const = 0;
    
    /**
     * @brief Gets the current position
     * @return The current geographical coordinates
     */
    virtual QGeoCoordinate position() const = 0;

    /**
     * @brief Gets the current UAS state
     * @return The current UAS state
     */
    UASState::State state() const;

    /**
     * @brief Gets the current loiter radius
     * @return Loiter radius in meters
     */
    virtual int loiterRadius() const = 0;
    
    /**
     * @brief Sets the loiter radius
     * @param radius Loiter radius in meters
     */
    virtual void setLoiterRadius(int radius) = 0;

    /**
     * @brief Gets the current loiter direction
     * @return true for clockwise, false for counter-clockwise
     */
    virtual bool loiterClockwise() const = 0;
    
    /**
     * @brief Sets the loiter direction
     * @param clockwise true for clockwise, false for counter-clockwise
     */
    virtual void setLoiterClockwise(bool clockwise) = 0;

    /**
     * @brief Command the UAS to take off
     */
    Q_INVOKABLE void takeOff();
    
    /**
     * @brief Command the UAS to land
     */
    Q_INVOKABLE void land();
    
    /**
     * @brief Command the UAS to fly
     */
    Q_INVOKABLE void fly();

    /**
     * @brief Command the UAS to fly to a specific destination
     * @param destination The geographical coordinates to fly to
     */
    Q_INVOKABLE virtual void goTo(const QGeoCoordinate& destination) = 0;

signals:
    /**
     * @brief Emitted when battery level changes
     * @param battery The new battery level
     */
    void batteryChanged(int battery);
    
    /**
     * @brief Emitted when altitude changes
     * @param altitude The new altitude
     */
    void altitudeChanged(int altitude);
    
    /**
     * @brief Emitted when speed changes
     * @param speed The new speed
     */
    void speedChanged(int speed);
    
    /**
     * @brief Emitted when position changes
     * @param position The new position
     */
    void positionChanged(QGeoCoordinate position);
    
    /**
     * @brief Emitted when UAS state changes
     * @param state The new UAS state
     */
    void stateChanged(UASState::State state);
    
    /**
     * @brief Emitted when loiter radius changes
     * @param radius The new loiter radius
     */
    void loiterRadiusChanged(int radius);
    
    /**
     * @brief Emitted when loiter direction changes
     * @param clockwise The new loiter direction (true=clockwise, false=counter-clockwise)
     */
    void loiterClockwiseChanged(bool clockwise);

protected:
    /** @brief The UAS state machine instance */
    UASStateMachine* m_stateMachine;
};

#endif // TELEMETRYDATA_HPP
