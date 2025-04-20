import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import GroundControlStation 1.0

Item {
    id: mapWidgetRoot
    
    // Expose some properties for parent component to control
    property bool interactive: true
    property var selectedDestination: null
    
    // Function to handle map clicks in navigation mode
    signal handleMapClick(var coordinate)
    
    Plugin {
        id: mapPlugin
        name: "osm" // OpenStreetMap
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: TelemetryData.position
        zoomLevel: MapController.zoomLevel

        // Keep the map centered on the UAS position
        Connections {
            target: TelemetryData
            function onPositionChanged() {
                // Only auto-center when not in navigation mode
                if (!MapController.isInteractive) {
                    map.center = TelemetryData.position
                }
            }
        }

        Behavior on zoomLevel {
            NumberAnimation {
                duration: 1000
            }
        }

        // Animation for center coordinate changes
        Behavior on center {
            CoordinateAnimation {
                duration: 1000
            }
        }

        // Handle tap/click on the map
        MouseArea {
            anchors.fill: parent
            enabled: MapController.isInteractive // Always enabled to capture clicks

            onClicked: {
                // Convert screen position (mouse x,y) to geographic coordinates
                var coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                console.log("Tapped at coordinate:", coordinate.latitude, coordinate.longitude)

                if (MapController.isInteractive) {
                    // In navigation mode, use the coordinate for destination
                    mapWidgetRoot.handleMapClick(coordinate)
                    
                    // Add a temporary marker at the clicked location
                    destinationMarker.coordinate = coordinate
                    MapController.targetCoordinates = coordinate
                    destinationMarker.visible = true
                    map.center = coordinate

                } else {
                    // Normal mode, just center the map
                    map.center = coordinate
                }
            }
        }

        Text
        {
            text: "Select a point on the map"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.margins: 10
            font.pixelSize: 32
            visible: MapController.isInteractive

            Rectangle
            {
                anchors.fill: parent
                color: "gray"
                opacity: .5
                radius: 5
            }
        }

        Button {
            id: zoomInBtn
            text: "+"
            onClicked: MapController.zoomLevel = MapController.zoomLevel + 1
            anchors.right: parent.right
            anchors.bottom: zoomOutBtn.top
            anchors.margins: 10
            width: height
            font.pixelSize: 32

            background: Rectangle {
                color:  "dark gray"
                radius: 25
            }
        }

        Button {
            id: zoomOutBtn
            text: "-"
            onClicked: MapController.zoomLevel = MapController.zoomLevel - 1
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
            width: height
            font.pixelSize: 32

            background: Rectangle {
                color:  "dark gray"
                radius: 25
            }
        }

        // UAS position marker
        MapQuickItem {
            id: uasMarker
            anchorPoint.x: uasIcon.width/2
            anchorPoint.y: uasIcon.height/2
            coordinate: TelemetryData.position

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
        
        MapQuickItem {
            id: destinationMarker
            visible: MapController.isInteractive
            anchorPoint.x: sourceItem.width / 2
            anchorPoint.y: sourceItem.height
            
            sourceItem: Item {
                width: 40
                height: 40
                
                Image
                {
                    id: destIcon
                    width: 50
                    height: 50
                    source: "/images/Waypoint.png"
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        
        // Path to destination when navigating
        MapPolyline {
            id: navPath
            line.width: 4
            line.color: "#de2828"
            opacity: .5
            visible: UASState.FlyingToWaypoint === TelemetryData.state
            path: [
                TelemetryData.position,
                destinationMarker.coordinate
            ]
        }
    }
}
