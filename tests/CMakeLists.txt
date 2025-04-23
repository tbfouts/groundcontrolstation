cmake_minimum_required(VERSION 3.16)

project(GroundControlStationTests LANGUAGES CXX)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Test Core Positioning)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Include parent project for headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/backend)

# Collect source files from parent project needed for testing
set(GCS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/UASStateMachine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/UASStateMachine.cpp
)

set(GCS_SIMULATOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/UASStateMachine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/UASStateMachine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/TelemetryData.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/TelemetryData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/TelemetryDataSimulator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/backend/TelemetryDataSimulator.cpp
)

# Create UASStateMachine test executable
qt_add_executable(testUASStateMachine
    TestUASStateMachine.cpp
    ${GCS_SOURCES}
)

# Create TelemetryDataSimulator test executable
qt_add_executable(testTelemetryDataSimulator
    TestTelemetryDataSimulator.cpp
    ${GCS_SIMULATOR_SOURCES}
)

# Link test libraries
target_link_libraries(testUASStateMachine PRIVATE
    Qt6::Test
    Qt6::Core
)

target_link_libraries(testTelemetryDataSimulator PRIVATE
    Qt6::Test
    Qt6::Core
    Qt6::Positioning
)

# Enable testing
enable_testing()

# Add tests to CTest
add_test(NAME UASStateMachineTest COMMAND testUASStateMachine)
add_test(NAME TelemetryDataSimulatorTest COMMAND testTelemetryDataSimulator)