import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Window {
    id: window
    width: 1920
    height: 1080
    minimumHeight: 720
    minimumWidth: 960
    visible: true
    title: "GCS"
    visibility: Qt.platform.os === "android" ? Window.FullScreen : Window.AutomaticVisibility

    Rectangle
    {
        id: root
        anchors.fill: parent
        color: "#363434"

        MapWidget
        {
            id: mapWidget
            height: parent.height * 0.45
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 20
            anchors.right: parent.right
            anchors.left: parent.left
        }

        TelemetryWidget
        {
            id: telemetryWidget
            width: parent.width * 0.3
            height: parent.height * 0.45
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 20
            radius: 24
        }

        StatusWidget
        {
            id: statusWidget
            width: parent.width * 0.3
            height: parent.height * 0.45
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.margins: 20
            radius: 24
        }

        ControlsWidget
        {
            id: controlsWidget
            width: parent.width * 0.3
            height: parent.height * 0.45
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            radius: 24
            
            // Connect to control actions
            onRequestConfirmation: {
                confirmationPopup.actionText = action
                confirmationPopup.visible = true
            }
        }
        
        // Overlay for the confirmation slider popup
        Rectangle {
            id: confirmationPopup
            anchors.fill: parent
            visible: false
            color: Qt.rgba(0, 0, 0, 0.7) // Semi-transparent black overlay
            z: 10 // Ensure it appears above other elements
            
            // Close when clicking outside the slider
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    confirmationPopup.visible = false
                    controlsWidget.cancelConfirmation()
                }
            }
            
            // Center the confirmation slider
            Rectangle {
                id: popupContainer
                width: parent.width * 0.5
                height: 300
                radius: 10
                color: "#1a1a1a"
                border.color: "#4d4d4d"
                border.width: 1
                anchors.centerIn: parent
                
                // Prevent clicks from reaching the background
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // Consume the event
                        mouse.accepted = true
                    }
                }
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 10
                    
                    Text {
                        Layout.fillWidth: true
                        text: "CONFIRM " + confirmationPopup.actionText
                        color: "#ffffff"
                        font.pixelSize: 18
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                    }
                    
                    ConfirmationSlider {
                        id: confirmationSlider
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        actionText: confirmationPopup.actionText
                        
                        onConfirmed: {
                            confirmationPopup.visible = false
                            controlsWidget.executeConfirmedAction(confirmationPopup.actionText)
                        }
                    }
                }
            }
            
            // Property to track which action is being confirmed
            property string actionText: ""
        }
    }

}
