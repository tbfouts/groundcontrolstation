#include "TelemetryDataSimulator.hpp"
#include <QDebug>
#include <QQmlEngine>
#include <QJSEngine>
#include <QtMath>

/**
 * @brief Constructs a TelemetryDataSimulator with default values
 * @param parent The parent QObject
 * 
 * Initializes the simulator with the following default values:
 * - Battery: 100%
 * - Altitude: 0 meters
 * - Speed: 0 m/s
 * - Position: Detroit, MI (42.3314, -83.0458)
 * - Direction: 45 degrees
 * - Loiter radius: 100 meters
 * - Loiter direction: Clockwise
 */
TelemetryDataSimulator::TelemetryDataSimulator(QObject* parent)
    : TelemetryData(parent)
    , m_random(QRandomGenerator::global()->generate())
    , m_battery(100)
    , m_altitude(0)
    , m_speed(0)
    , m_position(42.3314, -83.0458) // Default position: Detroit, MI
    , m_direction(45)
    , m_loiterRadius(100)
    , m_loiterClockwise(true)
{
    // Connect to state changes
    connect(m_stateMachine, &UASStateMachine::currentStateChanged,
            this, &TelemetryDataSimulator::handleStateChange);
}

/**
 * @brief Destructor
 */
TelemetryDataSimulator::~TelemetryDataSimulator()
{
}

/**
 * @brief Gets the current simulated battery level
 * @return Battery percentage (0-100)
 */
int TelemetryDataSimulator::battery() const
{
    return m_battery;
}

/**
 * @brief Gets the current simulated altitude
 * @return Altitude in meters
 */
int TelemetryDataSimulator::altitude() const
{
    return m_altitude;
}

/**
 * @brief Gets the current simulated speed
 * @return Speed in meters per second
 */
int TelemetryDataSimulator::speed() const
{
    return m_speed;
}

/**
 * @brief Gets the current simulated position
 * @return The current geographical coordinates
 */
QGeoCoordinate TelemetryDataSimulator::position() const
{
    return m_position;
}

/**
 * @brief Gets the current loiter radius
 * @return Loiter radius in meters
 */
int TelemetryDataSimulator::loiterRadius() const
{
    return m_loiterRadius;
}

/**
 * @brief Sets the loiter radius
 * @param radius The new loiter radius in meters
 * 
 * If the UAS is currently in loitering state, the loiter pattern
 * will be immediately updated to use the new radius.
 */
void TelemetryDataSimulator::setLoiterRadius(int radius)
{
    if (m_loiterRadius != radius) {
        m_loiterRadius = radius;
        emit loiterRadiusChanged(m_loiterRadius);
        
        // If currently loitering, update the loiter pattern with the new radius
        if (m_stateMachine->currentState() == UASState::Loitering) {
            simulateLoitering(m_destinationCoordinate);
        }
    }
}

/**
 * @brief Gets the current loiter direction
 * @return true for clockwise, false for counter-clockwise
 */
bool TelemetryDataSimulator::loiterClockwise() const
{
    return m_loiterClockwise;
}

/**
 * @brief Sets the loiter direction
 * @param clockwise true for clockwise, false for counter-clockwise
 * 
 * If the UAS is currently in loitering state, the loiter pattern
 * will be immediately updated to use the new direction.
 */
void TelemetryDataSimulator::setLoiterClockwise(bool clockwise)
{
    if (m_loiterClockwise != clockwise) {
        m_loiterClockwise = clockwise;

        qDebug() << "Loiter clockwise:" << m_loiterClockwise;
        emit loiterClockwiseChanged(m_loiterClockwise);
        
        // If currently loitering, update the loiter pattern with the new direction
        if (m_stateMachine->currentState() == UASState::Loitering) {
            simulateLoitering(m_destinationCoordinate);
        }
    }
}

/**
 * @brief Commands the simulated UAS to fly to a destination
 * @param destination The geographical coordinates to fly to
 * 
 * If the UAS is currently loitering, it will switch to flying mode
 * before navigating to the new destination.
 */
void TelemetryDataSimulator::goTo(const QGeoCoordinate& destination)
{
    // Only allow goTo when flying or loitering
    UASState::State currentState = m_stateMachine->currentState();
    // if (currentState != UASState::Flying && currentState != UASState::Loitering) {
    //     qDebug() << "Cannot navigate to destination: UAS must be flying or loitering";
    //     return;
    // }
    
    qDebug() << "Navigating to:" << destination.latitude() << destination.longitude();
    
    // Switch to flying state if currently loitering
    if (currentState == UASState::Loitering) {
        m_stateMachine->fly();
    }
    
    // Implement navigation by calling flyToWaypoint
    flyToWaypoint(destination);
}

/**
 * @brief Handles state changes from the UAS state machine
 * @param newState The new state to transition to
 * 
 * This method initiates the appropriate simulation behavior
 * based on the new state of the UAS.
 */
void TelemetryDataSimulator::handleStateChange(UASState::State newState)
{
    qDebug() << "TelemetryDataSimulator: State changed to" << newState;

    // Handle state changes
    switch (newState) {
    case UASState::TakingOff:
        simulateTakeOff();
        break;

    case UASState::Landing:
        simulateLanding();
        break;

    case UASState::Loitering:
        simulateLoitering(m_destinationCoordinate);
        break;

    case UASState::FlyingToWaypoint:
        // This state is handled by flyToWaypoint
        break;

    case UASState::Flying:
        simulateFlying();
        break;

    case UASState::Landed:
        break;

    default:
        break;
    }
}

/**
 * @brief Creates a simulation timer with the specified interval
 * @param interval The timer interval in milliseconds
 * @return A configured QTimer pointer
 */
QTimer* TelemetryDataSimulator::createSimTimer(int interval)
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(interval);
    return timer;
}

/**
 * @brief Updates the simulated position based on current direction and speed
 * 
 * The position is updated by calculating the movement in both latitude and
 * longitude based on the current speed and direction.
 */
void TelemetryDataSimulator::updatePosition()
{
    double radians = m_direction * M_PI / 180.0;
    double latChange = MOVEMENT_STEP * m_speed * cos(radians);
    double lonChange = MOVEMENT_STEP * m_speed * sin(radians);

    // Calculate new position
    double newLat = m_position.latitude() + latChange;
    double newLon = m_position.longitude() + lonChange;
    QGeoCoordinate newPosition(newLat, newLon);

    m_position = newPosition;
    emit positionChanged(m_position);
}

/**
 * @brief Simulates random battery drain
 * 
 * There is a 2% chance on each call that the battery will drain by 1%.
 * This is used to simulate gradual battery usage during flight.
 */
void TelemetryDataSimulator::drainBattery()
{
    // Exit early if battery is already at 0
    if (m_battery <= 0) {
        return;
    }

    // Generate a random number between 0.0 and 1.0
    double randomValue = m_random.generateDouble();

    // 2 percent chance of battery drain
    if (randomValue < 0.02) {
        m_battery--;
        emit batteryChanged(m_battery);
    }
}

/**
 * @brief Updates speed with linear interpolation
 * @param initialSpeed The starting speed
 * @param targetSpeed The target speed
 * @param progress The progress as a value from 0.0 to 1.0
 */
void TelemetryDataSimulator::updateSpeed(int initialSpeed, int targetSpeed, double progress)
{
    m_speed = initialSpeed + static_cast<int>((targetSpeed - initialSpeed) * progress);
    emit speedChanged(m_speed);
}

/**
 * @brief Updates altitude with linear interpolation
 * @param initialAltitude The starting altitude
 * @param targetAltitude The target altitude
 * @param progress The progress as a value from 0.0 to 1.0
 */
void TelemetryDataSimulator::updateAltitude(int initialAltitude, int targetAltitude, double progress)
{
    m_altitude = initialAltitude + static_cast<int>((targetAltitude - initialAltitude) * progress);
    emit altitudeChanged(m_altitude);
}

/**
 * @brief Applies random variations to flight parameters
 * 
 * Adds small random variations to speed and altitude to simulate
 * realistic flight behavior.
 */
void TelemetryDataSimulator::applyFlightVariations()
{
    // Small speed variation
    int speedAdjust = static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * 2.0);
    m_speed = qMax(38, qMin(42, m_speed + speedAdjust));
    emit speedChanged(m_speed);

    // Small altitude variation
    int altAdjust = static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * 3.0);
    m_altitude = qMax(100, qMin(135, m_altitude + altAdjust));
    emit altitudeChanged(m_altitude);
}

/**
 * @brief Simulates flying to a specific waypoint
 * @param destination The geographical coordinates to fly to
 * 
 * Sets up a timer that continuously updates the UAS position as it
 * flies towards the destination. When the destination is reached
 * (within 50 meters), the UAS will transition to loitering state.
 */
void TelemetryDataSimulator::flyToWaypoint(const QGeoCoordinate& destination)
{
    // Store the destination
    m_stateMachine->setCurrentState(UASState::FlyingToWaypoint);

    m_destinationCoordinate = destination;

    QTimer* flyToWaypointTimer = createSimTimer();
    
    connect(flyToWaypointTimer, &QTimer::timeout, this, [=]() mutable {
        // Check if we're still in a valid navigation state
        UASState::State currentState = m_stateMachine->currentState();
        if (currentState != UASState::Flying && 
            currentState != UASState::FlyingToWaypoint && 
            currentState != UASState::Loitering) {
            flyToWaypointTimer->stop();
            flyToWaypointTimer->deleteLater();
            flyToWaypointTimer = nullptr;
            qDebug() << "Navigation interrupted due to state change";
            return;
        }
        
        // Calculate bearing to the destination
        double bearing = m_position.azimuthTo(m_destinationCoordinate);
        
        // Calculate distance to the destination
        double distance = m_position.distanceTo(m_destinationCoordinate);
        
        // Set the direction towards the destination
        m_direction = bearing;
        
        // Check if we've reached the destination (within 50 meters)
        if (distance < 50) {
            qDebug() << "Reached destination:" << m_destinationCoordinate.latitude() << m_destinationCoordinate.longitude();
            
            // Switch to loitering state when destination reached
            simulateLoitering(destination);
            
            // Stop the navigation timer
            flyToWaypointTimer->stop();
            flyToWaypointTimer->deleteLater();
            return;
        }
        
        // Update position
        updatePosition();
        
        // Drain battery
        drainBattery();
    });
    
    flyToWaypointTimer->start();
}

/**
 * @brief Simulates the takeoff sequence
 * 
 * Initiates a simulation of the UAS taking off from the ground.
 * The sequence includes gradual acceleration and altitude increase
 * over the defined TAKEOFF_LANDING_DURATION. When complete, the UAS
 * transitions to the Flying state.
 */
void TelemetryDataSimulator::simulateTakeOff()
{
    m_direction = m_random.bounded(360);

    // Setup takeoff timer
    QTimer* takeoffTimer = createSimTimer();

    // Initialize takeoff parameters
    int elapsedTime = 0;

    // Connect timer to lambda function
    connect(takeoffTimer, &QTimer::timeout, this, [=]() mutable {
        // Update elapsed time
        elapsedTime += 250;

        // Calculate progress
        double progress = static_cast<double>(elapsedTime) / TAKEOFF_LANDING_DURATION;

        // Update speed - gradually accelerate to takeoff speed
        updateSpeed(m_speed, 40, progress);

        // Begin altitude increase after rotation speed
        if (m_speed > 10) {
            updateAltitude(m_altitude, 120, progress);
        }

        // Drain battery and update position
        drainBattery();
        updatePosition();

        // End takeoff sequence after duration
        if (elapsedTime >= TAKEOFF_LANDING_DURATION) {
            takeoffTimer->stop();
            takeoffTimer->deleteLater();

            // Transition to Flying state
            m_stateMachine->setCurrentState(UASState::Flying);

            qDebug() << "Takeoff sequence completed - Altitude:" << m_altitude
                     << "Speed:" << m_speed
                     << "Position:" << m_position.latitude() << m_position.longitude();
        }
    });

    // Start the takeoff timer
    takeoffTimer->start();
    qDebug() << "Starting takeoff sequence";
}

/**
 * @brief Simulates the landing sequence
 * 
 * Initiates a simulation of the UAS landing on the ground.
 * The sequence includes gradual deceleration and altitude decrease
 * over the defined TAKEOFF_LANDING_DURATION. When complete, the UAS
 * transitions to the Landed state.
 */
void TelemetryDataSimulator::simulateLanding()
{
    // Create landing timer
    QTimer* landingTimer = createSimTimer();

    int elapsedTime = 0;

    connect(landingTimer, &QTimer::timeout, this, [=]() mutable {
        // Update elapsed time
        elapsedTime += 250;

        // Calculate progress as a percentage (0.0 to 1.0)
        double progress = static_cast<double>(elapsedTime) / TAKEOFF_LANDING_DURATION;

        updateSpeed(m_speed, 0, progress);
        updateAltitude(m_altitude, 0, progress);
        updatePosition();
        drainBattery();

        // Complete landing when done
        if (elapsedTime >= TAKEOFF_LANDING_DURATION) {
            // Ensure values are exactly zero
            m_speed = 0;
            m_altitude = 0;
            emit speedChanged(m_speed);
            emit altitudeChanged(m_altitude);

            // Stop the landing timer
            landingTimer->stop();
            landingTimer->deleteLater();

            // Transition to Landed state
            m_stateMachine->setCurrentState(UASState::Landed);
        }
    });

    // Start the landing timer
    landingTimer->start();
}

/**
 * @brief Simulates loitering around a center point
 * @param centerPoint The center point of the loiter pattern
 * 
 * Initiates a simulation of the UAS loitering (circling) around the
 * specified center point. The radius and direction of the loiter pattern
 * are determined by the m_loiterRadius and m_loiterClockwise properties.
 * 
 * The UAS maintains a lower speed while loitering and keeps a more
 * consistent altitude than during normal flight.
 */
void TelemetryDataSimulator::simulateLoitering(const QGeoCoordinate& centerPoint)
{
    m_stateMachine->loiter();

    // Pre-calculate all points on the circle
    QVector<QGeoCoordinate> circlePoints;
    QVector<int> directions;

    // Convert radius in meters to degrees
    double lat = centerPoint.latitude();
    double lon = centerPoint.longitude();
    double latRadius = m_loiterRadius / 111000.0; // 1 degree lat is about 111km
    double lonRadius = m_loiterRadius / (111000.0 * cos(lat * M_PI / 180.0)); // Adjust for longitude

    // Calculate 360 points around the circle
    for (int angle = 0; angle < 360; angle++) {
        double radians = angle * M_PI / 180.0;

        // Calculate position on the circle
        double newLat = lat + latRadius * cos(radians);
        double newLon = lon + lonRadius * sin(radians);

        // Add point to the vector
        circlePoints.append(QGeoCoordinate(newLat, newLon));

        // Direction is tangent to the circle
        // If clockwise, add 90 degrees, if counterclockwise, subtract 90
        int directionOffset = m_loiterClockwise ? 90 : -90;
        int tangentDirection = static_cast<int>(fmod(angle + directionOffset, 360.0));
        directions.append(tangentDirection);
    }

    // Create loiter timer
    QTimer* loiterTimer = createSimTimer();
    int currentPointIndex = 0;

    connect(loiterTimer, &QTimer::timeout, this, [=]() mutable {
        // Check if we're still loitering
        if (m_stateMachine->currentState() != UASState::Loitering) {
            loiterTimer->stop();
            loiterTimer->deleteLater();
            return;
        }

        // Get pre-calculated position and direction
        m_position = circlePoints[currentPointIndex];
        m_direction = directions[currentPointIndex];

        // Update index for next point (circular)
        // Direction depends on the loiter direction
        if (m_loiterClockwise) {
            currentPointIndex = (currentPointIndex + 1) % 360;
        } else {
            currentPointIndex = (currentPointIndex - 1 + 360) % 360;
        }

        // Emit position change
        emit positionChanged(m_position);

        // Maintain altitude within a tighter range
        int altAdjust = static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * 1.0);
        m_altitude = qMax(100, qMin(110, m_altitude + altAdjust));
        emit altitudeChanged(m_altitude);

        // Maintain lower speed for loitering
        int speedAdjust = static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * 1.0);
        m_speed = qMax(15, qMin(20, m_speed + speedAdjust));
        emit speedChanged(m_speed);

        // Drain battery
        drainBattery();
    });

    // Start loitering
    loiterTimer->start();
}

/**
 * @brief Simulates normal flying behavior
 * 
 * Initiates a simulation of the UAS flying in normal cruise mode.
 * The speed and altitude are maintained within standard cruise ranges
 * with small random variations to simulate realistic flight behavior.
 * This simulation continues until the UAS state changes.
 */
void TelemetryDataSimulator::simulateFlying()
{
    // Create flight timer
    QTimer* flightTimer = createSimTimer();

    // Setup transition duration
    int elapsedTime = 0;
    const int TRANSITION_DURATION = 3000; // 3 seconds to stabilize at cruise

    connect(flightTimer, &QTimer::timeout, this, [=]() mutable {

        // Check if state has changed to landing - interrupt if so
        if (m_stateMachine->currentState() == UASState::Landing)
        {
            flightTimer->stop();
            flightTimer->deleteLater();
            qDebug() << "Flight interrupted - transitioning to landing";
            return;
        }

        // Update elapsed time
        elapsedTime += 250;

        applyFlightVariations();

        // Update position and battery
        updatePosition();
        drainBattery();
    });

    // Start the flight timer
    flightTimer->start();
}
