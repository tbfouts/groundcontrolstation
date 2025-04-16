import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: controlsWidget
    color: "#1a1a1a"
    radius: 10
    border.color: "#2a2a2a"
    border.width: 1
    
    property int labelTextSize: 24
    property color buttonHighlightColor: "#4dff64"  // Green highlight
    property color buttonNormalColor: "#333333"     // Dark gray normal state
    property color buttonHoverColor: "#444444"      // Slightly lighter on hover
    property color buttonPressedColor: "#222222"    // Darker when pressed
    
    // Track the current action to be confirmed
    property string pendingAction: ""
    
    // Signal to request confirmation from Main.qml
    signal requestConfirmation(string action)
    
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
            Layout.preferredHeight: parent.height * 0.28
            buttonText: "TAKE OFF"
            iconSource: "/images/TakeOffIcon.png"
            enabled: pendingAction === "" || pendingAction === "TAKE OFF"

            onClicked: {
                if (pendingAction === "") {
                    pendingAction = "TAKE OFF"
                    requestConfirmation("TAKE OFF")
                } else {
                    pendingAction = ""
                }
            }
        }

        ControlButton {
            id: landButton
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.28
            buttonText: "LAND"
            iconSource: "/images/LandIcon.png"
            enabled: pendingAction === "" || pendingAction === "LAND"

            onClicked: {
                if (pendingAction === "") {
                    pendingAction = "LAND"
                    requestConfirmation("LAND")
                } else {
                    pendingAction = ""
                }
            }
        }

        ControlButton {
            id: gotoButton
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.28
            buttonText: "GO TO"
            iconSource: "/images/GoToIcon.png"
            enabled: pendingAction === "" || pendingAction === "GO TO"

            onClicked: {
                if (pendingAction === "") {
                    pendingAction = "GO TO"
                    requestConfirmation("GO TO")
                } else {
                    pendingAction = ""
                }
            }
        }
        
        // Spacer to fill remaining space
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
    
    // Timer to reset pending action state
    Timer {
        id: actionClearTimer
        interval: 3000
        repeat: false
        onTriggered: {
            pendingAction = ""
        }
    }
    
    // Function to execute the confirmed action - called by Main.qml
    function executeConfirmedAction(action) {
        console.log("Executing action: " + action)
        
        // Update button states
        if (action === "TAKE OFF") {
            takeoffButton.isActive = true
            landButton.isActive = false
            gotoButton.isActive = false
            console.log("Aircraft taking off")
        } else if (action === "LAND") {
            takeoffButton.isActive = false
            landButton.isActive = true
            gotoButton.isActive = false
            console.log("Aircraft landing")
        } else if (action === "GO TO") {
            takeoffButton.isActive = false
            landButton.isActive = false
            gotoButton.isActive = true
            console.log("Navigating to selected point")
        }
        
        // Clear pending action after a delay
        actionClearTimer.start()
    }
    
    // Function to cancel the current confirmation - called by Main.qml
    function cancelConfirmation() {
        pendingAction = ""
    }
}
