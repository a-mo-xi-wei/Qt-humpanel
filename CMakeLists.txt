cmake_minimum_required(VERSION 3.10)

project(humpanel VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Charts)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB SOURCE_framelesscore_CPP ${CMAKE_CURRENT_SOURCE_DIR}/framelesscore/*.cpp)
file(GLOB SOURCE_framelesscore_H ${CMAKE_CURRENT_SOURCE_DIR}/framelesscore/*.h)

set(SOURCE_FILES
        ${SOURCE_H}
        ${SOURCE_framelesscore_H}
        ${SOURCE_CPP}
        ${SOURCE_framelesscore_CPP}
        img.qrc
        MainWindow.ui
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Charts)
