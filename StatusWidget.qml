import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: statusWidget
    color: "#1a1a1a"
    radius: 10
    border.color: "#2a2a2a"
    border.width: 1
    
    // Properties for UAS status
    property string connectionStatus: "Connected" // "Connected", "Disconnected", "Connecting"
    property string operationalMode: "Manual" // "Manual", "Autonomous", "Return to Home"
    property string flightStatus: "Landed" // "Landed", "Flying", "Takeoff", "Landing"
    property int signalStrength: 85 // 0-100
    property bool isArmed: false
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 5
        
        Text {
            text: "UAS STATUS"
            color: "#ffffff"
            font.pixelSize: 22
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "CONNECTION"
            value: connectionStatus
            
            // Choose color based on connection status
            valueColor: {
                if (connectionStatus === "Connected") return "#4dff64"
                if (connectionStatus === "Connecting") return "#ffcc00"
                return "#ff4d4d"
            }
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "SIGNAL STRENGTH"
            value: signalStrength + "%"
            valueColor: {
                if (signalStrength > 70) return "#4dff64"
                if (signalStrength > 40) return "#ffcc00"
                return "#ff4d4d"
            }
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "MODE"
            value: operationalMode
            valueColor: "#ffffff"
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "FLIGHT STATUS"
            value: flightStatus
            valueColor: {
                if (flightStatus === "Landed") return "#4dff64"
                if (flightStatus === "Flying") return "#3cc3ff"
                if (flightStatus === "Takeoff") return "#ffcc00"
                if (flightStatus === "Landing") return "#ffcc00"
                return "#ffffff"
            }
        }
        
        DataLabel {
            Layout.fillWidth: true
            label: "ARM STATUS"
            value: isArmed ? "ARMED" : "DISARMED"
            valueColor: isArmed ? "#ff4d4d" : "#4dff64"
        }
        
        // Animation to simulate status changes
        Timer {
            interval: 5000
            running: true
            repeat: true
            
            onTriggered: {
                // Simulate random status changes for demonstration
                var r = Math.random()
                
                if (r > 0.8) {
                    statusWidget.connectionStatus = statusWidget.connectionStatus === "Connected" ? 
                        "Connecting" : (statusWidget.connectionStatus === "Connecting" ? 
                        "Connected" : "Disconnected")
                } else if (r > 0.6) {
                    statusWidget.signalStrength = Math.min(100, Math.max(0, 
                        statusWidget.signalStrength + (Math.random() > 0.5 ? 10 : -10)))
                } else if (r > 0.4) {
                    statusWidget.operationalMode = ["Manual", "Autonomous", "Return to Home"][Math.floor(Math.random() * 3)]
                } else if (r > 0.2) {
                    statusWidget.flightStatus = ["Landed", "Flying", "Takeoff", "Landing"][Math.floor(Math.random() * 4)]
                } else {
                    statusWidget.isArmed = !statusWidget.isArmed
                }
            }
        }
    }
}
