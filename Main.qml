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
        }
    }

}
