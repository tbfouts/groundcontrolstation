#include "MapController.hpp"
#include <QQmlEngine>
#include <QJSEngine>

/**
 * @brief Constructs a MapController with default settings
 * @param parent The parent QObject
 * 
 * Initializes the controller with default values:
 * - Interactive mode: false
 * - Target coordinates: (0, 0)
 * - Zoom level: 15
 */
MapController::MapController(QObject* parent)
    : QObject(parent)
    , m_isInteractive(false)
    , m_targetCoordinates(0, 0)  // Default to (0,0)
    , m_zoomLevel(15)  // Default zoom level
{
}

/**
 * @brief Destructor
 */
MapController::~MapController()
{
}

/**
 * @brief Gets whether the map is in interactive mode
 * @return true if the map is interactive, false otherwise
 */
bool MapController::isInteractive() const
{
    return m_isInteractive;
}

/**
 * @brief Gets the current target coordinates of the map
 * @return The geographical coordinates at the center of the map
 */
QGeoCoordinate MapController::targetCoordinates() const
{
    return m_targetCoordinates;
}

/**
 * @brief Gets the current zoom level of the map
 * @return The zoom level value (1-20)
 */
int MapController::zoomLevel() const
{
    return m_zoomLevel;
}

/**
 * @brief Sets whether the map is in interactive mode
 * @param interactive true to make the map interactive, false otherwise
 * 
 * Emits isInteractiveChanged if the value changes.
 */
void MapController::setIsInteractive(bool interactive)
{
    if (m_isInteractive != interactive) {
        m_isInteractive = interactive;
        emit isInteractiveChanged(m_isInteractive);
    }
}

/**
 * @brief Sets the target coordinates of the map
 * @param coordinates The geographical coordinates to center the map on
 * 
 * Emits targetCoordinatesChanged if the value changes.
 */
void MapController::setTargetCoordinates(const QGeoCoordinate& coordinates)
{
    if (m_targetCoordinates != coordinates) {
        m_targetCoordinates = coordinates;
        emit targetCoordinatesChanged(m_targetCoordinates);
    }
}

/**
 * @brief Sets the zoom level of the map
 * @param level The zoom level (1-20, where 1 is most zoomed out)
 * 
 * Bounds the level to the valid range of 1-20 and emits
 * zoomLevelChanged if the value changes.
 */
void MapController::setZoomLevel(int level)
{
    // Ensure zoom level is within reasonable bounds (1-20)
    int boundedLevel = qBound(1, level, 20);
    
    if (m_zoomLevel != boundedLevel) {
        m_zoomLevel = boundedLevel;
        emit zoomLevelChanged(m_zoomLevel);
    }
}

/**
 * @brief Factory method for creating a QML singleton instance
 * @param qmlEngine The QML engine
 * @param jsEngine The JavaScript engine
 * @return A new MapController instance owned by the QML engine
 * 
 * This method is used by the QML engine to create a singleton instance
 * of the MapController for use in QML.
 */
MapController* MapController::create(QQmlEngine* qmlEngine, QJSEngine* jsEngine)
{
    // QML engine is responsible for deleting this object
    MapController* controller = new MapController();
    return controller;
}
