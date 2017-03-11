cmake_minimum_required(VERSION 2.8.12)

project(demo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab dlab.cpp)
target_link_libraries(lab ${OpenCV_LIBS})

add_executable(demo2 demo2.cpp)
target_link_libraries(demo2 ${OpenCV_LIBS})
