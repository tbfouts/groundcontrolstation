#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MapController.hpp"
#include "TelemetryData.hpp"
#include "TelemetryDataSimulator.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
    // Create the telemetry simulator
    auto* telemetrySimulator = new TelemetryDataSimulator();

    auto* mapController = new MapController();

    // Register the UASState enum type with QML
    qmlRegisterUncreatableType<UASState>("GroundControlStation", 1, 0, "UASState", "UASState is an enum type, not creatable");
    
    // Register simulator instance as the TelemetryData singleton in QML
    qmlRegisterSingletonInstance<TelemetryData>("GroundControlStation", 1, 0, "TelemetryData", telemetrySimulator);

    // Register mapcontroller instance as a QML singleton
    qmlRegisterSingletonInstance<MapController>("GroundControlStation", 1, 0, "MapController", mapController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("GroundControlStation", "Main");

    return app.exec();
}
