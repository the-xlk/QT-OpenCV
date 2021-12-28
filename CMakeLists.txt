cmake_minimum_required(VERSION 3.5)

project(ClassifierExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# OpenCV directory
set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv-4.5.4/build)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# QT Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(ClassifierExample main.cpp mainwindow.h mainwindow.cpp mainwindow.ui)

# Link required libraries
target_link_libraries(ClassifierExample Qt6::Core Qt6::Gui Qt6::Widgets ${OpenCV_LIBS})



