cmake_minimum_required(VERSION 3.2.2)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME luminator2k)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH luminator2k.ino)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)
set(${CMAKE_PROJECT_NAME}_SERIAL picocom @SERIAL_PORT@ -b 57600 -l)
set(${CMAKE_PROJECT_NAME}_BOARD nano328)


generate_arduino_firmware(${CMAKE_PROJECT_NAME})

