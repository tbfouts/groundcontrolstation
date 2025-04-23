#ifndef TELEMETRYDATASIMULATOR_HPP
#define TELEMETRYDATASIMULATOR_HPP

#include "TelemetryData.hpp"
#include <QTimer>
#include <QRandomGenerator>

/**
 * @class TelemetryDataSimulator
 * @brief Simulates telemetry data for testing and development
 *
 * This class provides a simulated implementation of the TelemetryData interface.
 * It generates realistic telemetry data including position, altitude, speed,
 * and battery levels, and simulates different flight behaviors such as takeoff,
 * landing, flying to waypoints, and loitering.
 */
class TelemetryDataSimulator : public TelemetryData
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a TelemetryDataSimulator
     * @param parent The parent QObject
     */
    explicit TelemetryDataSimulator(QObject* parent = nullptr);
    
    /**
     * @brief Constructs a TelemetryDataSimulator with a provided state machine
     * @param stateMachine External state machine to use
     * @param parent The parent QObject
     */
    explicit TelemetryDataSimulator(UASStateMachine* stateMachine, QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    virtual ~TelemetryDataSimulator();

    // TelemetryData interface implementation
    /**
     * @brief Gets the simulated battery level
     * @return Battery percentage (0-100)
     */
    int battery() const override;
    
    /**
     * @brief Gets the simulated altitude
     * @return Altitude in meters
     */
    int altitude() const override;
    
    /**
     * @brief Gets the simulated speed
     * @return Speed in meters per second
     */
    int speed() const override;
    
    /**
     * @brief Gets the simulated position
     * @return Current geographical coordinates
     */
    QGeoCoordinate position() const override;

    /**
     * @brief Gets the target altitude
     * @return Altitude in meters
     */
    int targetAltitude() const override;


    /**
     * @brief Sets the target altitude
     * @return Altitude in meters
     */
    void setTargetAltitude(const int altitude) override;
    
    /**
     * @brief Command the UAS to take off
     */
    Q_INVOKABLE virtual void takeOff() override;

    /**
     * @brief Command the UAS to land
     */
    Q_INVOKABLE virtual void land() override;

    /**
     * @brief Command the UAS to fly to a specific destination
     * @param destination The geographical coordinates to fly to
     * @param loiterRadius The radius size for loitering
     * @param loiterClockwise True if the UAS should loiter clockwise, false if it should loiter counterclockwise
     */
    Q_INVOKABLE virtual void goTo(const QGeoCoordinate& destination, const int loiterRadius, const bool loiterClockwise) override;

private:
    /**
     * @brief Simulates loitering around a center point
     * @param centerPoint The center of the loiter pattern
     */
    void simulateLoitering(const QGeoCoordinate& centerPoint, const int loiterRadius, const bool loiterClockwise);
    
    /**
     * @brief Simulates normal flying behavior
     */
    void simulateFlying();

    /**
     * @brief Creates a simulation timer
     * @param interval The timer interval in milliseconds
     * @return A configured QTimer pointer
     */
    QTimer* createSimTimer(int interval = 250);
    
    /**
     * @brief Updates the simulated position based on speed and direction
     */
    void updatePosition();
    
    /**
     * @brief Simulates random battery drain
     */
    void drainBattery();
    
    /**
     * @brief Updates the speed using linear interpolation
     * @param initialSpeed The starting speed
     * @param targetSpeed The target speed
     * @param progress The progress value (0.0 to 1.0)
     */
    void updateSpeed(int initialSpeed, int targetSpeed, double progress);
    
    /**
     * @brief Updates the altitude using linear interpolation
     * @param initialAltitude The starting altitude
     * @param targetAltitude The target altitude
     * @param progress The progress value (0.0 to 1.0)
     */
    void updateAltitude(int initialAltitude, int targetAltitude, double progress);
    
    /**
     * @brief Applies small random variations to flight parameters for realistic simulation
     */
    void applyFlightVariations();
    
    /** @brief The destination coordinates for navigation */
    QGeoCoordinate m_destinationCoordinate;

    /** @brief Random number generator for simulation variations */
    QRandomGenerator m_random;
    
    /** @brief Simulated battery level (percentage) */
    int m_battery;
    
    /** @brief Simulated altitude (meters) */
    int m_altitude;

    /** @brief Target altitude (meters) as defined by the user */
    int m_target_altitude;
    
    /** @brief Simulated speed (meters per second) */
    int m_speed;
    
    /** @brief Simulated position */
    QGeoCoordinate m_position;
    
    /** @brief Current direction in degrees (0-359) */
    int m_direction;
    
    /** @brief Movement step size in degrees per update */
    const double MOVEMENT_STEP = 0.00001;
    
    /** @brief Duration of takeoff and landing sequences in milliseconds */
    const int TAKEOFF_LANDING_DURATION = 7000;
};

#endif // TELEMETRYDATASIMULATOR_HPP
