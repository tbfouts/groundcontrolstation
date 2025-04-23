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
    Q_PROPERTY(int targetAltitude READ targetAltitude WRITE setTargetAltitude NOTIFY targetAltitudeChanged)

public:
    /**
     * @brief Constructs a TelemetryData object
     * @param parent The parent QObject
     */
    explicit TelemetryData(QObject* parent = nullptr);
    
    /**
     * @brief Constructs a TelemetryData object with a provided state machine
     * @param stateMachine External state machine to use
     * @param parent The parent QObject
     */
    explicit TelemetryData(UASStateMachine* stateMachine, QObject* parent = nullptr);
    
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
     * @brief Gets the target altitude
     * @return Altitude in meters
     */
    virtual int targetAltitude() const = 0;


    /**
     * @brief Sets the target altitude
     * @return Altitude in meters
     */
    virtual void setTargetAltitude(const int altitude) = 0;

    /**
     * @brief Command the UAS to take off
     */
    Q_INVOKABLE virtual void takeOff();
    
    /**
     * @brief Command the UAS to land
     */
    Q_INVOKABLE virtual void land();

    /**
     * @brief Command the UAS to fly to a specific destination
     * @param destination The geographical coordinates to fly to
     * @param loiterRadius The radius size for loitering
     * @param loiterClockwise True if the UAS should loiter clockwise, false if it should loiter counterclockwise
     */
    Q_INVOKABLE virtual void goTo(const QGeoCoordinate& destination, const int loiterRadius, const bool loiterClockwise);

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

    /**
     * @brief Emitted when target altitude changes
     * @param altitude The new target altitude
     */
    void targetAltitudeChanged(int altitude);
protected:
    /** @brief The UAS state machine instance */
    UASStateMachine* m_stateMachine;
};

#endif // TELEMETRYDATA_HPP
