#include "TelemetryDataSimulator.hpp"
#include <QDebug>
#include <QQmlEngine>
#include <QJSEngine>
#include <QtMath>

TelemetryDataSimulator::TelemetryDataSimulator(QObject* parent)
    : TelemetryData(parent)
    , m_random(QRandomGenerator::global()->generate())
    , m_battery(100)
    , m_altitude(0)
    , m_speed(0)
    , m_position(42.3314, -83.0458) // Default position: Detroit, MI
    , m_direction(m_random.bounded(360)) // Random initial direction
{
    // Connect timer to update method
    connect(&m_updateTimer, &QTimer::timeout, this, &TelemetryDataSimulator::updateTelemetry);
    
    // Set default update interval
    m_updateTimer.setInterval(DEFAULT_UPDATE_INTERVAL);
}

TelemetryDataSimulator::~TelemetryDataSimulator()
{
    stop();
}

int TelemetryDataSimulator::battery() const
{
    return m_battery;
}

int TelemetryDataSimulator::altitude() const
{
    return m_altitude;
}

int TelemetryDataSimulator::speed() const
{
    return m_speed;
}

QGeoCoordinate TelemetryDataSimulator::position() const
{
    return m_position;
}

void TelemetryDataSimulator::start()
{
    if (!m_updateTimer.isActive()) {
        m_updateTimer.start();
    }
}

void TelemetryDataSimulator::stop()
{
    if (m_updateTimer.isActive()) {
        m_updateTimer.stop();
    }
}

void TelemetryDataSimulator::setUpdateInterval(int msec)
{
    m_updateTimer.setInterval(msec > 0 ? msec : DEFAULT_UPDATE_INTERVAL);
}

void TelemetryDataSimulator::updateTelemetry()
{
    // Log current values for debugging
    qDebug() << "Current telemetry - Battery:" << m_battery << "Altitude:" << m_altitude 
             << "Speed:" << m_speed << "Position:" << m_position.latitude() << m_position.longitude();
             
    // Update battery (slowly decreasing)
    if (m_battery > 0) {
        // 0.1% chance of dropping 1%
        if (static_cast<int>(m_random.generateDouble() * 1000) < 1) {
            m_battery--;
            emit batteryChanged(m_battery);
        }
    }
    
    // Update altitude with random changes
    int newAltitude = m_altitude + static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * MAX_ALTITUDE_CHANGE);
    newAltitude = qMax(0, newAltitude); // Don't go below 0
    
    if (m_altitude != newAltitude) {
        m_altitude = newAltitude;
        emit altitudeChanged(m_altitude);
    }
    
    // Update speed with smaller random changes for smoothness
    int speedChange = static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * 2.0);
    int newSpeed = m_speed + speedChange;
    newSpeed = qMax(0, qMin(40, newSpeed)); // Keep between 0-40 m/s
    
    if (m_speed != newSpeed) {
        m_speed = newSpeed;
        emit speedChanged(m_speed);
    }
    
    // Possibly change direction (small probability)
    if (static_cast<int>(m_random.generateDouble() * 100) < DIRECTION_CHANGE_PROBABILITY) {
        // Change direction by -45 to +45 degrees
        int directionChange = static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * 45);
        m_direction = (m_direction + directionChange + 360) % 360;
        qDebug() << "Direction changed to:" << m_direction << "degrees";
    }
    
    // Move in the current direction
    double radians = m_direction * M_PI / 180.0;
    double latChange = MOVEMENT_STEP * cos(radians);
    double lonChange = MOVEMENT_STEP * sin(radians);
    
    // Calculate new position based on direction
    double newLat = m_position.latitude() + latChange;
    double newLon = m_position.longitude() + lonChange;
    
    // Create and set the new position
    QGeoCoordinate newPosition(newLat, newLon);
    if (m_position != newPosition) {
        m_position = newPosition;
        qDebug() << "Position updated to:" << newPosition.latitude() << newPosition.longitude() 
                 << "Direction:" << m_direction << "Speed:" << m_speed;
        emit positionChanged(m_position);
    }
}
