import QtQuick
import QtLocation
import QtPositioning
import GroundControlStation 1.0

Item
{
    // Access telemetry data from C++ singleton
    property var telemetry: UncrewedAerialSystem.telemetry
    
    // Default position in case telemetry is unavailable
    readonly property var defaultPosition: QtPositioning.coordinate(42.331429, -83.045753) // Detroit
    
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
        center: telemetry && telemetry.position ? telemetry.position : defaultPosition
        zoomLevel: 13

        // Animation for center coordinate changes
        Behavior on center {
            CoordinateAnimation {
                duration: 1000
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
            coordinate: telemetry && telemetry.position ? telemetry.position : defaultPosition

            // Animation for center coordinate changes
            Behavior on coordinate {
                CoordinateAnimation {
                    duration: 1000
                }
            }

            sourceItem: Rectangle {
                id: uasIcon
                color: "#de2828"
                width: 32
                height: 32
                radius: 16
                opacity: .9
            }
        }
    }
}
