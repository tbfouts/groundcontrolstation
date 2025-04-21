# Ground Control Station (GCS)

A simplified Ground Control Station (GCS) application prototype that allows users to monitor and control a simulated Uncrewed Aerial System (UAS).

## Project Structure

The project is organized into the following structure:

```
/
├── CMakeLists.txt       # Main CMake build file
├── main.cpp             # Application entry point
├── resources.qrc        # Resource file for QML and images
├── images/              # Images for the application
├── src/
│   ├── backend/         # C++ backend code
│   │   ├── TelemetryData.hpp/cpp           # Base telemetry data interface
│   │   ├── TelemetryDataSimulator.hpp/cpp  # Simulated telemetry implementation
│   │   ├── UASStateMachine.hpp/cpp         # UAS state machine
│   │   └── MapController.hpp/cpp           # Map display controller
│   └── frontend/        # QML frontend code
│       ├── Main.qml                        # Application main window
│       ├── MapWidget.qml                   # Map display widget
│       ├── TelemetryWidget.qml             # Telemetry display widget
│       ├── ControlsWidget.qml              # Control buttons widget
│       ├── StatusWidget.qml                # UAS status widget
│       ├── ControlButton.qml               # Control button component
│       ├── MapButton.qml                   # Map control button component
│       ├── DataLabel.qml                   # Telemetry data label component
│       └── ConfirmationSlider.qml          # Slider with confirmation
└── build/               # Build output directory
```

## Building the Project

1. Make sure you have Qt 6.8 or later installed with the Quick and Location modules.
2. Configure the project with CMake:
   ```
   cmake .
   ```
3. Build the project:
   ```
   make
   ```
4. Run the application:
   ```
   ./appGroundControlStation.app/Contents/MacOS/appGroundControlStation
   ```

## Features

- Map display with UAS location tracking
- Real-time telemetry data display (altitude, speed, battery)
- Takeoff and landing controls
- Go-to waypoint navigation
- Loitering functionality with configurable radius and direction
