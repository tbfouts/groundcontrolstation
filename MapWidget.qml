import QtQuick
import QtLocation
import QtPositioning

Item
{
    Plugin
    {
        id: mapPlugin
        name: "osm" // OpenStreetMap
    }

    Map
    {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(42.331429, -83.045753) // Detroit coordinates
        zoomLevel: 10

        // Animation for center coordinate changes
        Behavior on center {
            CoordinateAnimation {
                duration: 1000  // Animation duration in milliseconds
                easing.type: Easing.OutCubic  // Smooth deceleration curve
            }
        }

        // Handle tap/click on the map
        MouseArea {
            anchors.fill: parent

            onClicked: {
                // Convert screen position (mouse x,y) to geographic coordinates
                var coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))

                console.log("Tapped at coordinate:", coordinate.latitude, coordinate.longitude)

                map.center = coordinate
            }
        }

        MapQuickItem {
            id: uasMarker
            anchorPoint.x: uasIcon.width/2
            anchorPoint.y: uasIcon.height/2
            coordinate: map.center  // Update with actual UAS position

            sourceItem: Rectangle {
                id: uasIcon
                color: "red"
                radius: 25
                opacity: .8
                width: 32
                height: 32
            }
        }
    }
}