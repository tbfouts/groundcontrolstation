import QtQuick

Rectangle
{
    id: dataLabel
    height: 30
    color: "#252525"
    radius: 6

    property string label: "LABEL"
    property string value: "Value"
    property color valueColor: "#ffffff"

    // Label text
    Text {
        text: label
        color: "#FFFFFF"
        font.pixelSize: 12
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: 10
    }

    // Value text
    Text {
        text: value
        color: valueColor
        font.pixelSize: 14
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: 10
    }
}
