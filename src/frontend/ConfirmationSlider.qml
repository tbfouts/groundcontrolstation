import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: confirmationSlider
    color: "#1a1a1a"
    radius: 8
    height: 60
    clip: true
    
    // Public properties
    property string actionText: "CONFIRM ACTION"
    property int confirmThreshold: 100 // Percentage needed to confirm (0-100)
    
    // Signals
    signal confirmed()
    
    // Container for the slider
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5
        
        // Slider for confirmation
        Slider {
            id: slider
            Layout.fillWidth: true
            Layout.fillHeight: true
            from: 0
            to: 100
            value: 0

            // Custom handle that shows an arrow icon
            handle: Rectangle {
                x: slider.leftPadding + (slider.visualPosition * (slider.availableWidth - width))
                y: slider.topPadding + (slider.availableHeight - height) / 2
                width: height
                height: confirmationSlider.height / 2
                radius: 25
                color: "#ffffff"
                border.color: "#dddddd"

                // Arrow icon
                Text {
                    anchors.centerIn: parent
                    text: "→"
                    font.pixelSize: 20
                    font.bold: true
                    color: "#333333"
                }
            }
            
            // Custom background for the slider track
            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + (slider.availableHeight - height) / 2
                width: slider.availableWidth
                height: 10
                radius: 5
                color: "#333333"
                
                // Colored portion that shows progress
                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    radius: 5
                    color: slider.value >= confirmThreshold ? "#4dff64" : "#3cc3ff"
                }
            }
            
            // Handle drag completion - trigger confirmation when exceeding threshold
            onPressedChanged: {
                if (!pressed && value >= confirmThreshold) {
                    confirmed();
                    
                    // Reset after a delay
                    resetTimer.start();
                } else if (!pressed) {
                    // Spring back to start if released below threshold
                    resetAnimation.start();
                }
            }
        }
        
        // Instruction text
        Text {
            id: instructionText
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            color: "#ffffff"
            font.pixelSize: 14
            font.bold: true
            text: {
                if (slider.value >= confirmThreshold) 
                    return "RELEASE TO CONFIRM";
                else if (slider.value > 0)
                    return "SLIDE TO CONFIRM";
                else
                    return "SLIDE RIGHT TO " + actionText;
            }
        }
    }
    
    // Animation to reset slider to 0
    NumberAnimation {
        id: resetAnimation
        target: slider
        property: "value"
        to: 0
        duration: 300
        easing.type: Easing.OutBack
    }
    
    // Timer to reset after confirmation
    Timer {
        id: resetTimer
        interval: 500
        onTriggered: resetAnimation.start()
    }
}
