import QtQuick
import QtQuick.Controls

Button {
    property string iconSource: ""

    height: 60
    width: height

    background: Rectangle {
        color:  "dark gray"
        radius: 30

        Image {
            source: iconSource
            anchors.centerIn: parent
        }
    }
}
