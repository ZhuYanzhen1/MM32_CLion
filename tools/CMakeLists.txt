cmake_minimum_required(VERSION 3.5)

project(monitor LANGUAGES CXX)

file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(app_WINRC app_win32.rc)

find_package(Qt5 COMPONENTS Widgets SerialPort PrintSupport Network REQUIRED)

add_definitions(-DQSU_INCLUDE_MOC=1)

set(QRC_SOURCE_FILES updater/qsimpleupdater.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
SOURCE_GROUP("Resource Files" FILES ${QRC_SOURCE_FILES})

set_property(SOURCE app_win32.rc APPEND PROPERTY OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/icon.ico)

include_directories(updater form protocol)

file(GLOB_RECURSE PROJECT_SOURCES "form/*.cpp" "updater/*.cpp" "form/*.ui" "updater/*.ui" "updater/*.h" "form/*.h"
    "protocol/*.cpp")

add_executable(monitor WIN32 ${app_WINRC} ${PROJECT_SOURCES} ${QRC_FILES})

target_link_libraries(monitor Qt5::Widgets Qt5::SerialPort Qt5::PrintSupport Qt5::Network)
