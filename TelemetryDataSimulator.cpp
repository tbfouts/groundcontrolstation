#include "TelemetryDataSimulator.hpp"

TelemetryDataSimulator::TelemetryDataSimulator(QObject* parent)
    : TelemetryData(parent)
    , m_random(QRandomGenerator::global()->generate())
    , m_battery(100)
    , m_altitude(0)
    , m_speed(0)
    , m_position(42.3314, -83.0458) // Default position: Detroit, MI
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
    
    // Update speed with random changes
    int newSpeed = m_speed + static_cast<int>((m_random.generateDouble() * 2.0 - 1.0) * MAX_SPEED_CHANGE);
    newSpeed = qMax(0, newSpeed); // Don't go below 0
    
    if (m_speed != newSpeed) {
        m_speed = newSpeed;
        emit speedChanged(m_speed);
    }
    
    // Update position with small random changes
    double newLat = m_position.latitude() + (m_random.generateDouble() * 2.0 - 1.0) * MAX_LAT_LON_CHANGE;
    double newLon = m_position.longitude() + (m_random.generateDouble() * 2.0 - 1.0) * MAX_LAT_LON_CHANGE;
    
    QGeoCoordinate newPosition(newLat, newLon);
    if (m_position != newPosition) {
        m_position = newPosition;
        emit positionChanged(m_position);
    }
}