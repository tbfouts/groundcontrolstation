import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GroundControlStation 1.0

Rectangle {
    id: telemetryWidget
    color: "#1a1a1a"
    radius: 10
    border.color: "#2a2a2a"
    border.width: 1
    
    // Access telemetry data from C++ singleton
    property var telemetry: UncrewedAerialSystem.telemetry
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 5
        
        Text {
            text: "TELEMETRY"
            color: "#ffffff"
            font.pixelSize: 22
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "BATTERY"
            value: telemetry ? (telemetry.battery + "%") : "N/A"
            valueColor: {
                if (telemetry && telemetry.battery > 60) return "#4dff64"
                if (telemetry && telemetry.battery > 30) return "#ffcc00"
                return "#ff4d4d"
            }
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "ALTITUDE"
            value: telemetry ? (telemetry.altitude + " m") : "N/A"
            valueColor: "#3cc3ff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "SPEED"
            value: telemetry ? (telemetry.speed + " m/s") : "N/A"
            valueColor: "#3cc3ff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "LATITUDE"
            value: telemetry && telemetry.position ? (telemetry.position.latitude.toFixed(6) + "°") : "N/A"
            valueColor: "#ffffff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "LONGITUDE"
            value: telemetry && telemetry.position ? (telemetry.position.longitude.toFixed(6) + "°") : "N/A"
            valueColor: "#ffffff"
        }
    }
}
