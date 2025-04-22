cmake_minimum_required(VERSION 3.16)

project(GroundControlStationTests LANGUAGES CXX)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Test Core)

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

# Create test executable
qt_add_executable(testGroundControlStation
    TestUASStateMachine.cpp
    ${GCS_SOURCES}
)

# Link test libraries
target_link_libraries(testGroundControlStation PRIVATE
    Qt6::Test
    Qt6::Core
)

# Enable testing
enable_testing()

# Add test to CTest
add_test(NAME UASStateMachineTest COMMAND testGroundControlStation)