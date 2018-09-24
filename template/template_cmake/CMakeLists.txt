cmake_minimum_required(VERSION 2.8)

project(main)

find_package(OpenCV REQUIRED)

if(CMAKE_VERSION VERSION_LESS "2.8.11")
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
