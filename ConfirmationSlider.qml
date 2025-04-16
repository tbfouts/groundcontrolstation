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
    property bool isConfirmed: false
    property int confirmThreshold: 80 // Percentage needed to confirm (0-100)
    property color confirmColor: "#4dff64"  // Green for confirm
    property color pendingColor: "#ff9500"  // Orange for pending
    property color trackColor: "#333333"    // Dark gray for track
    
    // Private properties
    property bool _dragging: false
    property real _draggedPercentage: 0
    property bool _confirmationPending: false
    
    // Signals
    signal confirmed()
    
    // Reset state when action text changes
    onActionTextChanged: resetState()
    
    // Enable/disable animation based on state
    SequentialAnimation {
        id: blinkAnimation
        running: _confirmationPending
        loops: Animation.Infinite
        
        PropertyAnimation {
            target: confirmTrack
            property: "opacity"
            from: 1.0
            to: 0.6
            duration: 700
        }
        
        PropertyAnimation {
            target: confirmTrack
            property: "opacity"
            from: 0.6
            to: 1.0
            duration: 700
        }
    }
    
    // Background track
    Rectangle {
        id: sliderTrack
        anchors.fill: parent
        color: trackColor
        radius: parent.radius
        
        // Colored confirmation track that grows as slider moves
        Rectangle {
            id: confirmTrack
            width: parent.width * _draggedPercentage / 100
            height: parent.height
            color: _confirmationPending ? pendingColor : confirmColor
            radius: parent.radius
            
            Behavior on width {
                enabled: !_dragging
                NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
            }
        }
        
        // Slider handle
        Rectangle {
            id: sliderHandle
            width: height
            height: parent.height - 16
            radius: height / 2
            x: Math.min(parent.width - width - 4, Math.max(4, (_draggedPercentage / 100) * (parent.width - width - 8) + 4))
            y: 8
            color: "#ffffff"
            
            Behavior on x {
                enabled: !_dragging
                NumberAnimation { duration: 300; easing.type: Easing.OutBack }
            }
            
            // Handle shadow
            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                color: "#10000000"
                z: -1
                anchors.bottomMargin: -2
                anchors.rightMargin: -2
                anchors.leftMargin: 2
                anchors.topMargin: 2
            }
            
            // Arrow icon inside handle
            Text {
                anchors.centerIn: parent
                text: "→"
                color: "#333333"
                font.pixelSize: parent.height * 0.5
                font.bold: true
            }
        }
        
        // Instruction text
        Text {
            anchors.centerIn: parent
            text: _confirmationPending ? "RELEASE TO CONFIRM" : 
                                        (_draggedPercentage > 0 ? "SLIDE TO CONFIRM" : "SLIDE RIGHT TO " + actionText)
            color: "#ffffff"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            leftPadding: sliderHandle.width + 10
            rightPadding: sliderHandle.width + 10
        }
        
        // Touch/mouse area for drag interaction
        MouseArea {
            anchors.fill: parent
            
            onPressed: {
                _dragging = true
                updateDragPosition(mouseX)
            }
            
            onReleased: {
                _dragging = false
                
                if (_draggedPercentage >= confirmThreshold) {
                    confirmed()
                } else {
                    resetPosition()
                }
            }
            
            onPositionChanged: {
                if (_dragging) {
                    updateDragPosition(mouseX)
                }
            }
            
            onCanceled: {
                _dragging = false
                resetPosition()
            }
        }
    }
    
    // Timer to reset the slider after confirmation
    Timer {
        id: resetTimer
        interval: 2000
        repeat: false
        onTriggered: {
            resetState()
        }
    }
    
    // Reset position without resetting confirmation
    function resetPosition() {
        if (!_confirmationPending) {
            _draggedPercentage = 0
        }
    }
    
    // Reset the entire slider state
    function resetState() {
        _confirmationPending = false
        _draggedPercentage = 0
        isConfirmed = false
        resetTimer.stop()
    }
    
    // Cancel an in-progress confirmation
    function cancelConfirmation() {
        if (_confirmationPending) {
            _confirmationPending = false
            resetPosition()
            confirmationCancelled()
        }
    }
    
    // Update the drag position based on mouse x coordinate
    function updateDragPosition(mouseX) {
        if (_confirmationPending) {
            // Allow cancellation by dragging back
            if (mouseX < sliderTrack.width * 0.5) {
                cancelConfirmation()
            }
            return
        }
        
        // Calculate drag percentage
        var maxDragX = sliderTrack.width - sliderHandle.width - 8
        var dragPercentage = Math.max(0, Math.min(100, (mouseX - sliderHandle.width / 2) / maxDragX * 100))
        _draggedPercentage = dragPercentage
    }
}
