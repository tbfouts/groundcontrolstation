#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "UncrewedAerialSystem.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register the singleton instance
    UncrewedAerialSystem::registerQmlType();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("GroundControlStation", "Main");

    return app.exec();
}
