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
            value: TelemetryData.battery + "%"
            valueColor: {
                if (TelemetryData.battery > 60)
                    return "#4dff64"
                if (TelemetryData.battery > 30)
                    return "#ffcc00"
                return "#ff4d4d"
            }
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "ALTITUDE"
            value: TelemetryData.altitude + " m"
            valueColor: "#3cc3ff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "SPEED"
            value: TelemetryData.speed + " m/s"
            valueColor: "#3cc3ff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "LATITUDE"
            value: TelemetryData.position.latitude.toFixed(6) + "°"
            valueColor: "#ffffff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "LONGITUDE"
            value: TelemetryData.position.longitude.toFixed(6) + "°"
            valueColor: "#ffffff"
        }
    }
}
