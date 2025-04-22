# Ground Control Station (GCS)

A simplified Ground Control Station (GCS) application prototype that allows users to monitor and control a simulated Uncrewed Aerial System (UAS).

## Overview

This application provides a user interface for monitoring and controlling a UAS (drone) with features commonly found in professional ground control stations. The application is designed with a Qt Quick/QML frontend for the user interface, with C++ backend classes that handle the business logic and simulation of the UAS.

## Architecture

The application follows a Model-View-Controller pattern:

- **Model**: TelemetryData represents the core data model, with TelemetryDataSimulator providing a simulated implementation
- **View**: QML files in the frontend directory provide the user interface
- **Controller**: UASStateMachine manages state transitions, while UAS class acts as the main controller component

Key design patterns used:
- **State Machine**: UASStateMachine implements a state pattern to manage UAS flight states
- **Strategy Pattern**: The TelemetryData interface allows for different implementations (simulation vs real hardware)
- **Singleton**: UAS and other key components are exposed as QML singletons for easy access

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
│   │   ├── UASStateMachine.hpp/cpp         # UAS state machine interface
│   │   ├── UASStateMachineSimulator.hpp/cpp # Simulated state machine implementation
│   │   ├── UAS.hpp/cpp                     # Main UAS controller class
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
└── tests/               # Unit tests directory
    ├── CMakeLists.txt                      # Test build configuration
    ├── TestTelemetryDataSimulator.cpp      # Tests for telemetry simulator
    └── TestUASStateMachine.cpp             # Tests for state machine
```

## Building the Project

1. Make sure you have Qt 6.8 or later installed with the Quick and Location modules.
2. Configure the project with CMake:
   ```
   mkdir build && cd build
   cmake ..
   ```
3. Build the project:
   ```
   make
   ```
4. Run the application:
   ```
   ./appGroundControlStation
   ```

## Features

- Map display with UAS location tracking and zoom controls
- Real-time telemetry data display (altitude, speed, battery)
- UAS state visualization (Landed, TakingOff, Flying, etc.)
- Takeoff and landing controls with confirmation
- Go-to waypoint navigation by clicking on the map
- Loitering functionality with configurable radius and direction
- Simulated flight physics with realistic transitions

## Future Improvement Plans

### Architecture Improvements
- **Hardware Abstraction Layer**: Replace direct simulation with HAL to allow real hardware integration
- **Plugin System**: Create an architecture to support different vehicle types and communication protocols
- **Revamep Architecture**: Current architecture was designed with simulation in mind and has many limitations.

### Feature Enhancements
- **Mission Planning**: Add mission planning with multiple waypoints and mission items
- **Geofencing**: Implement geofencing with no-fly zones and containment areas
- **Multi-Vehicle Support**: Support controlling and monitoring multiple vehicles simultaneously
- **Logging System**: Implement flight data logging and replay functionality
- **3D Visualization**: Add 3D terrain and attitude visualization
- **Failsafe Behaviors**: Add configurable failsafe behaviors for low battery, lost link, etc.

### Technical Improvements
- **Test Coverage**: Expand unit tests to cover all components
- **CI/CD Pipeline**: Set up continuous integration and deployment
- **Documentation**: Generate API documentation with Doxygen
