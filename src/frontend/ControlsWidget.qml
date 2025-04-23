import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GroundControlStation 1.0

Rectangle {
    id: controlsWidget
    color: "#1a1a1a"
    radius: 10

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
            disabled: UASState.Flying === TelemetryData.state ||
                      UASState.FlyingToWaypoint === TelemetryData.state ||
                      UASState.Loitering === TelemetryData.state ||
                      UASState.TakingOff === TelemetryData.state

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
            disabled: UASState.Landed === TelemetryData.state ||
                      UASState.Landing === TelemetryData.state

            onConfirmed: {
                TelemetryData.land();
            }
        }

        ControlButton {
            id: gotoButton
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.25
            buttonText: "GO TO POINT"
            iconSource: "/images/GoToIcon.png"
            disabled: UASState.Landed === TelemetryData.state ||
                      UASState.Landing === TelemetryData.state ||
                      UASState.TakingOff === TelemetryData.state

            onInitialClick:
            {
                MapController.isInteractive = true
                goToWaypointConfirmation.visible = true
            }
        }
    }

    Rectangle
    {
        id: goToWaypointConfirmation
        color: "#1a1a1a"
        radius: 10
        anchors.fill: parent
        visible: false

        Column
        {
            anchors.centerIn: parent
            spacing: 2
            width: parent.width * .75

            Text {
                id: pointOnMapTxt
                text: qsTr("SELECT A POINT ON THE MAP")
                color: "white"
                font.pixelSize: 16
                font.bold: true
            }

            Text {
                id: latLonTxt
                text: MapController.targetCoordinates.latitude.toFixed(6) + "°, " + MapController.targetCoordinates.longitude.toFixed(6) + "°"
                color: "white"
                font.pixelSize: 12
                font.bold: true
            }

            Text {
                id: loiterRadiusTxt
                text: qsTr("LOITER RADIUS: " + Math.floor(loiterRadiusSlider.value) + "m")
                color: "white"
                font.pixelSize: 16
                font.bold: true
            }

            Slider
            {
                id: loiterRadiusSlider
                from: 5
                to: 250
                width: parent.width
            }

            Text {
                id: loiterDirectionTxt
                text: qsTr("LOITER DIRECTION:")
                color: "white"
                font.pixelSize: 16
                font.bold: true
            }

            Row
            {

                RadioButton
                {
                    id: clockWiseRadioBtn
                    text: "CLOCKWISE"
                    font.pixelSize: 12
                    font.bold: true
                    checked: counterClockWiseRadioBtn.checked
                }

                RadioButton
                {
                    id: counterClockWiseRadioBtn
                    text: "COUNTER CLOCKWISE"
                    font.pixelSize: 12
                    font.bold: true
                    checked: clockWiseRadioBtn.checked
                }
            }

            ConfirmationSlider
            {
                width: parent.width
                actionText: "GO TO WAYPOINT"

                onConfirmed:
                {
                    goToWaypointConfirmation.visible = false
                    gotoButton.showConfirmationSlider = false
                    TelemetryData.goTo(MapController.targetCoordinates, Math.floor(loiterRadiusSlider.value), clockWiseRadioBtn.checked)
                    MapController.isInteractive = false
                }
            }
        }
    }
}
