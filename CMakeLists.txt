cmake_minimum_required(VERSION 3.16)

project(GroundControlStation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Location)

qt_standard_project_setup(REQUIRES 6.8)

# Define backend source files
set(BACKEND_SOURCES
    src/backend/TelemetryData.hpp
    src/backend/TelemetryData.cpp
    src/backend/TelemetryDataSimulator.hpp
    src/backend/TelemetryDataSimulator.cpp
    src/backend/UASStateMachine.hpp
    src/backend/UASStateMachine.cpp
    src/backend/MapController.hpp
    src/backend/MapController.cpp
)

# Include source directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backend
)

qt_add_executable(appGroundControlStation
    main.cpp
    ${BACKEND_SOURCES}
)

qt_add_resources(appGroundControlStation "images"
    PREFIX "/"
    FILES
        images/UASicon.png
        images/TakeOffIcon.png
        images/LandIcon.png
        images/GoToIcon.png
        images/Waypoint.png
        images/SnapToUAS.png
        images/Plus.png
        images/Minus.png
)

# Define frontend QML files
set(FRONTEND_QML_FILES
    src/frontend/Main.qml
    src/frontend/MapWidget.qml
    src/frontend/TelemetryWidget.qml
    src/frontend/ControlsWidget.qml
    src/frontend/StatusWidget.qml
    src/frontend/ControlButton.qml
    src/frontend/DataLabel.qml
    src/frontend/ConfirmationSlider.qml
    src/frontend/MapButton.qml
)

qt_add_qml_module(appGroundControlStation
    URI GroundControlStation
    VERSION 1.0
    QML_FILES
        ${FRONTEND_QML_FILES}
    SOURCES
        ${BACKEND_SOURCES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGroundControlStation PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGroundControlStation
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGroundControlStation
    PRIVATE Qt6::Quick
    Qt6::Location
)

include(GNUInstallDirs)
install(TARGETS appGroundControlStation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
