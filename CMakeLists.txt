cmake_minimum_required(VERSION 3.16)

project(drawLine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 QUIET COMPONENTS Widgets)
if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_PACKAGE Qt5)
endif()

add_executable(drawLine
    main.cpp
    line_canvas_widget.cpp
    line_canvas_widget.h
)

target_link_libraries(drawLine PRIVATE ${QT_PACKAGE}::Widgets)
