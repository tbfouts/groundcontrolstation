import QtQuick
import QtQuick.Layouts

Rectangle {
    id: button
    radius: 10
    color: buttonNormalColor

    property string buttonText: "BUTTON"
    property string iconSource: ""
    property bool isActive: false

    signal clicked()

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
        hoverEnabled: true

        onClicked: button.clicked()

        // Visual feedback
        onPressed: {
            button.color = buttonPressedColor
        }

        onReleased: {
            if (containsMouse)
                button.color = buttonHoverColor
            else
                button.color = buttonNormalColor
        }

        onEntered: {
            if (!pressed)
                button.color = buttonHoverColor
        }

        onExited: {
            if (!pressed)
                button.color = buttonNormalColor
        }
    }

    // Color transition animation
    Behavior on color {
        ColorAnimation { duration: 150 }
    }
}
