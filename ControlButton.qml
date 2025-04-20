import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    
    // Public properties
    property string buttonText: "BUTTON"
    property string iconSource: ""
    property bool disabled: false

    // Emitted when the confirmation slider is confirmed
    signal confirmed()

    // Emitted when this button is clicked initially
    signal initialClick()

    // Whether or not the confirmation slider should be shown
    property bool showConfirmationSlider: false
    
    // Colors
    property color normalColor: "#333333"
    property color hoverColor: "#444444"
    property color pressedColor: "#222222"
    property color activeColor: "#4dff64"
    
    // Button itself
    Rectangle {
        id: buttonRect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height
        radius: 10

        // Dynamic color based on state
        color: mouseArea.containsPress ? pressedColor : normalColor
        opacity: disabled ? .5 : 1

        Text {
            text: buttonText
            color: "#ffffff"
            font.pixelSize: 18
            font.bold: true
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 20
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        
        Image {
            source: iconSource
            height: 30
            width: 30
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 20
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignVCenter
        }
        
        // Hover and pressed states
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            enabled: !disabled
            
            onClicked:
            {
                initialClick()
                showConfirmationSlider = !showConfirmationSlider
            }
        }
    }
    
    // Confirmation slider, loaded only when needed
    Loader {
        id: confirmLoader
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: showConfirmationSlider ? buttonRect.height : 0
        active: showConfirmationSlider
        visible: showConfirmationSlider

        sourceComponent: ConfirmationSlider {
            id: slider
            actionText: buttonText
            anchors.fill: parent
            
            onConfirmed: {
                root.confirmed();
                showConfirmationSlider = false;
            }
        }
    }
}
