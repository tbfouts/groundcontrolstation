import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GroundControlStation 1.0

Rectangle {
    id: controlsWidget
    color: "#1a1a1a"
    radius: 10
    border.color: "#2a2a2a"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 5

        Text {
            text: "FLIGHT CONTROLS"
            color: "#ffffff"
            font.pixelSize: 22
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        ControlButton {
            id: takeoffButton
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.25
            buttonText: "TAKE OFF"
            iconSource: "/images/TakeOffIcon.png"

            onConfirmed: {
                TelemetryData.takeOff();
            }
        }

        ControlButton {
            id: landButton
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.25
            buttonText: "LAND"
            iconSource: "/images/LandIcon.png"

            onConfirmed: {
                TelemetryData.land();
            }
        }

        ControlButton {
            id: gotoButton
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.25
            buttonText: "GO TO"
            iconSource: "/images/GoToIcon.png"

            onConfirmed: {
                console.log("Navigating to selected point");
            }
        }
    }
}
