import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: telemetryWidget
    color: "#1a1a1a"
    radius: 10
    border.color: "#2a2a2a"
    border.width: 1
    
    // Telemetry data properties
    property double batteryPercentage: 75
    property double altitude: 145.7
    property double speed: 12.3
    property double latitude: 42.331429
    property double longitude: -83.045753
    
    // Animation to simulate battery changes
    SequentialAnimation {
        running: true
        loops: Animation.Infinite
        
        NumberAnimation {
            target: telemetryWidget
            property: "batteryPercentage"
            from: 75
            to: 25
            duration: 12000
            easing.type: Easing.InOutQuad
        }
        
        NumberAnimation {
            target: telemetryWidget
            property: "batteryPercentage"
            from: 25
            to: 75
            duration: 12000
            easing.type: Easing.InOutQuad
        }
    }
    
    // Animation to simulate altitude changes
    SequentialAnimation {
        running: true
        loops: Animation.Infinite
        
        NumberAnimation {
            target: telemetryWidget
            property: "altitude"
            from: 145.7
            to: 210.3
            duration: 8000
            easing.type: Easing.InOutSine
        }
        
        NumberAnimation {
            target: telemetryWidget
            property: "altitude"
            from: 210.3
            to: 145.7
            duration: 8000
            easing.type: Easing.InOutSine
        }
    }
    
    // Animation to simulate speed changes
    SequentialAnimation {
        running: true
        loops: Animation.Infinite
        
        NumberAnimation {
            target: telemetryWidget
            property: "speed"
            from: 12.3
            to: 18.7
            duration: 5000
            easing.type: Easing.InOutQuad
        }
        
        NumberAnimation {
            target: telemetryWidget
            property: "speed"
            from: 18.7
            to: 12.3
            duration: 5000
            easing.type: Easing.InOutQuad
        }
    }
    
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
            value: Math.round(batteryPercentage) + "%"
            valueColor: {
                if (batteryPercentage > 60) return "#4dff64"
                if (batteryPercentage > 30) return "#ffcc00"
                return "#ff4d4d"
            }
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "ALTITUDE"
            value: altitude.toFixed(1) + " m"
            valueColor: "#3cc3ff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "SPEED"
            value: speed.toFixed(1) + " m/s"
            valueColor: "#3cc3ff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "LATITUDE"
            value: latitude.toFixed(6) + "°"
            valueColor: "#ffffff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "LONGITUDE"
            value: longitude.toFixed(6) + "°"
            valueColor: "#ffffff"
        }
    }
}
