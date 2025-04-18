import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import GroundControlStation 1.0

Rectangle {
    id: statusWidget
    color: "#1a1a1a"
    radius: 10
    border.color: "#2a2a2a"
    border.width: 1
        
    // Simulated properties
    property string connectionStatus: "Connected" // "Connected", "Disconnected", "Connecting"
    property string operationalMode: "Manual" // "Manual", "Autonomous", "Return to Home"
    property int signalStrength: 85 // 0-100
    
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
        }
        
        DataLabel
        {
            Layout.fillWidth: true
            label: "SIGNAL STRENGTH"
            value: signalStrength + "%"
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
            value: {
                switch(TelemetryData.state)
                {
                    case UASState.Landed:
                        return "Landed"
                    case UASState.TakingOff:
                        return "Taking Off"
                    case UASState.Flying:
                        return "Flying"
                    case UASState.Loitering:
                        return "Loitering"
                    case UASState.Landing:
                        return "Landing"
                    default:
                        return "Unknown"
                }
            }
            valueColor: {
                switch(TelemetryData.state)
                {
                    case UASState.Landed:
                        return "#4dff64"
                    case UASState.Flying: 
                    case UASState.Loitering:
                        return "#3cc3ff"
                    case UASState.TakingOff:
                    case UASState.Landing:
                        return "#ffcc00"
                    default:
                        return "#ffffff"
                }
            }
        }
    }
}
