#ifndef TELEMETRYDATASIMULATOR_HPP
#define TELEMETRYDATASIMULATOR_HPP

#include "TelemetryData.hpp"
#include <QTimer>
#include <QRandomGenerator>

class TelemetryDataSimulator : public TelemetryData
{
    Q_OBJECT

public:
    explicit TelemetryDataSimulator(QObject* parent = nullptr);
    virtual ~TelemetryDataSimulator();

    // TelemetryData interface implementation
    int battery() const override;
    int altitude() const override;
    int speed() const override;
    QGeoCoordinate position() const override;

    // Control methods
    void start();
    void stop();
    void setUpdateInterval(int msec);

private slots:
    void updateTelemetry();

private:
    QTimer m_updateTimer;
    QRandomGenerator m_random;
    
    int m_battery;
    int m_altitude;
    int m_speed;
    QGeoCoordinate m_position;
    
    // Movement direction (in degrees, 0-359)
    int m_direction;
    
    // Simulation parameters
    static constexpr int DEFAULT_UPDATE_INTERVAL = 100; // ms
    static constexpr double MAX_ALTITUDE_CHANGE = 10.0;  // meters
    static constexpr double MAX_SPEED_CHANGE = 5.0;      // m/s
    static constexpr double MOVEMENT_STEP = 0.0005;      // degrees per update
    static constexpr int DIRECTION_CHANGE_PROBABILITY = 2; // % chance to change direction per update
};

#endif // TELEMETRYDATASIMULATOR_HPP
