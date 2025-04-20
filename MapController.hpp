#ifndef MAPCONTROLLER_HPP
#define MAPCONTROLLER_HPP

#include <QObject>
#include <QGeoCoordinate>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlregistration.h>

class MapController : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(bool isInteractive READ isInteractive WRITE setIsInteractive NOTIFY isInteractiveChanged)
    Q_PROPERTY(QGeoCoordinate targetCoordinates READ targetCoordinates WRITE setTargetCoordinates NOTIFY targetCoordinatesChanged)
    Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)
    
public:
    explicit MapController(QObject* parent = nullptr);
    ~MapController();
    
    // Property getters
    bool isInteractive() const;
    QGeoCoordinate targetCoordinates() const;
    int zoomLevel() const;
    
    // Property setters
    void setIsInteractive(bool interactive);
    void setTargetCoordinates(const QGeoCoordinate& coordinates);
    void setZoomLevel(int level);
    
    // Factory method for QML singleton
    static MapController* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);
    
signals:
    void isInteractiveChanged(bool interactive);
    void targetCoordinatesChanged(const QGeoCoordinate& coordinates);
    void zoomLevelChanged(int level);
    
private:
    bool m_isInteractive;
    QGeoCoordinate m_targetCoordinates;
    int m_zoomLevel;
};

#endif // MAPCONTROLLER_HPP
