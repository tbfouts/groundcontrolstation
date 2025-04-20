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
    void goTo(const QGeoCoordinate& destination) override;

private slots:
    void handleStateChange(UASState::State newState);

private:
    // Control methods for specific states
    void simulateTakeOff();
    void simulateLanding();
    void simulateLoitering(const QGeoCoordinate& centerPoint, double radius = 100);
    void simulateFlying();

    // Helper methods for simulating data
    QTimer* createSimTimer(int interval = 250);
    void updatePosition();
    void drainBattery();
    void updateSpeed(int initialSpeed, int targetSpeed, double progress);
    void updateAltitude(int initialAltitude, int targetAltitude, double progress);
    void applyFlightVariations();
    void flyToWaypoint(const QGeoCoordinate& destination);
    
    // Navigation members
    QGeoCoordinate m_destinationCoordinate;

    QRandomGenerator m_random;
    
    int m_battery;
    int m_altitude;
    int m_speed;
    QGeoCoordinate m_position;
    int m_direction;
    
    const double MOVEMENT_STEP = 0.00001;      // degrees per update
    const int TAKEOFF_LANDING_DURATION = 7000;
};

#endif // TELEMETRYDATASIMULATOR_HPP
