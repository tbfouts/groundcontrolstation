#ifndef MAPCONTROLLER_HPP
#define MAPCONTROLLER_HPP

#include <QObject>
#include <QGeoCoordinate>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlregistration.h>

/**
 * @class MapController
 * @brief Controls the map display and interaction settings
 * 
 * This class manages the map view settings, including the target coordinates,
 * zoom level, and whether the map is in interactive mode. It's designed to be
 * used as a singleton in QML.
 */
class MapController : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(bool isInteractive READ isInteractive WRITE setIsInteractive NOTIFY isInteractiveChanged)
    Q_PROPERTY(QGeoCoordinate targetCoordinates READ targetCoordinates WRITE setTargetCoordinates NOTIFY targetCoordinatesChanged)
    Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)
    
public:
    /**
     * @brief Constructs a MapController object
     * @param parent The parent QObject
     */
    explicit MapController(QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~MapController();
    
    /**
     * @brief Gets whether the map is in interactive mode
     * @return true if the map is interactive, false otherwise
     * 
     * When the map is interactive, the user can pan and zoom it directly.
     * When not interactive, the map is controlled programmatically.
     */
    bool isInteractive() const;
    
    /**
     * @brief Gets the current target coordinates of the map
     * @return The geographical coordinates at the center of the map
     */
    QGeoCoordinate targetCoordinates() const;
    
    /**
     * @brief Gets the current zoom level of the map
     * @return The zoom level value (1-20)
     */
    int zoomLevel() const;
    
    /**
     * @brief Sets whether the map is in interactive mode
     * @param interactive true to make the map interactive, false otherwise
     */
    void setIsInteractive(bool interactive);
    
    /**
     * @brief Sets the target coordinates of the map
     * @param coordinates The geographical coordinates to center the map on
     */
    void setTargetCoordinates(const QGeoCoordinate& coordinates);
    
    /**
     * @brief Sets the zoom level of the map
     * @param level The zoom level (1-20, where 1 is most zoomed out)
     * 
     * The level will be bounded to the valid range of 1-20.
     */
    void setZoomLevel(int level);
    
    /**
     * @brief Factory method for creating a QML singleton instance
     * @param qmlEngine The QML engine
     * @param jsEngine The JavaScript engine
     * @return A new MapController instance owned by the QML engine
     */
    static MapController* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);
    
signals:
    /**
     * @brief Emitted when the interactive mode changes
     * @param interactive The new interactive mode state
     */
    void isInteractiveChanged(bool interactive);
    
    /**
     * @brief Emitted when the target coordinates change
     * @param coordinates The new target coordinates
     */
    void targetCoordinatesChanged(const QGeoCoordinate& coordinates);
    
    /**
     * @brief Emitted when the zoom level changes
     * @param level The new zoom level
     */
    void zoomLevelChanged(int level);
    
private:
    /** @brief Whether the map is in interactive mode */
    bool m_isInteractive;
    
    /** @brief The current target coordinates of the map */
    QGeoCoordinate m_targetCoordinates;
    
    /** @brief The current zoom level of the map (1-20) */
    int m_zoomLevel;
};

#endif // MAPCONTROLLER_HPP
