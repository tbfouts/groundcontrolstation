#include "MapController.hpp"
#include <QQmlEngine>
#include <QJSEngine>

MapController::MapController(QObject* parent)
    : QObject(parent)
    , m_isInteractive(false)
    , m_targetCoordinates(0, 0)  // Default to (0,0)
    , m_zoomLevel(15)  // Default zoom level
{
}

MapController::~MapController()
{
}

bool MapController::isInteractive() const
{
    return m_isInteractive;
}

QGeoCoordinate MapController::targetCoordinates() const
{
    return m_targetCoordinates;
}

int MapController::zoomLevel() const
{
    return m_zoomLevel;
}

void MapController::setIsInteractive(bool interactive)
{
    if (m_isInteractive != interactive) {
        m_isInteractive = interactive;
        emit isInteractiveChanged(m_isInteractive);
    }
}

void MapController::setTargetCoordinates(const QGeoCoordinate& coordinates)
{
    if (m_targetCoordinates != coordinates) {
        m_targetCoordinates = coordinates;
        emit targetCoordinatesChanged(m_targetCoordinates);
    }
}

void MapController::setZoomLevel(int level)
{
    // Ensure zoom level is within reasonable bounds (1-20)
    int boundedLevel = qBound(1, level, 20);
    
    if (m_zoomLevel != boundedLevel) {
        m_zoomLevel = boundedLevel;
        emit zoomLevelChanged(m_zoomLevel);
    }
}

// Factory method for QML singleton
MapController* MapController::create(QQmlEngine* qmlEngine, QJSEngine* jsEngine)
{
    // QML engine is responsible for deleting this object
    MapController* controller = new MapController();
    return controller;
}
