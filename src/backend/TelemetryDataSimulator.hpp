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
     * @brief Commands the simulated UAS to fly to a destination
     * @param destination The geographical coordinates to fly to
     */
    void goTo(const QGeoCoordinate& destination) override;
    
    // Loiter properties implementation
    /**
     * @brief Gets the loiter radius for the simulated UAS
     * @return Loiter radius in meters
     */
    int loiterRadius() const override;
    
    /**
     * @brief Sets the loiter radius for the simulated UAS
     * @param radius Loiter radius in meters
     */
    void setLoiterRadius(int radius) override;
    
    /**
     * @brief Gets the loiter direction for the simulated UAS
     * @return true for clockwise, false for counter-clockwise
     */
    bool loiterClockwise() const override;
    
    /**
     * @brief Sets the loiter direction for the simulated UAS
     * @param clockwise true for clockwise, false for counter-clockwise
     */
    void setLoiterClockwise(bool clockwise) override;

private slots:
    /**
     * @brief Handles state changes from the UAS state machine
     * @param newState The new state to handle
     */
    void handleStateChange(UASState::State newState);

private:
    /**
     * @brief Simulates the takeoff sequence
     */
    void simulateTakeOff();
    
    /**
     * @brief Simulates the landing sequence
     */
    void simulateLanding();
    
    /**
     * @brief Simulates loitering around a center point
     * @param centerPoint The center of the loiter pattern
     */
    void simulateLoitering(const QGeoCoordinate& centerPoint);
    
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
    
    /**
     * @brief Simulates flying to a waypoint
     * @param destination The destination coordinates
     */
    void flyToWaypoint(const QGeoCoordinate& destination);
    
    /** @brief The destination coordinates for navigation */
    QGeoCoordinate m_destinationCoordinate;

    /** @brief Random number generator for simulation variations */
    QRandomGenerator m_random;
    
    /** @brief Simulated battery level (percentage) */
    int m_battery;
    
    /** @brief Simulated altitude (meters) */
    int m_altitude;
    
    /** @brief Simulated speed (meters per second) */
    int m_speed;
    
    /** @brief Simulated position */
    QGeoCoordinate m_position;
    
    /** @brief Current direction in degrees (0-359) */
    int m_direction;
    
    /** @brief Loiter radius in meters */
    int m_loiterRadius;
    
    /** @brief Loiter direction (true=clockwise, false=counter-clockwise) */
    bool m_loiterClockwise;
    
    /** @brief Movement step size in degrees per update */
    const double MOVEMENT_STEP = 0.00001;
    
    /** @brief Duration of takeoff and landing sequences in milliseconds */
    const int TAKEOFF_LANDING_DURATION = 7000;
};

#endif // TELEMETRYDATASIMULATOR_HPP
