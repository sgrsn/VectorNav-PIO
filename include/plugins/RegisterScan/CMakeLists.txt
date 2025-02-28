cmake_minimum_required(VERSION 3.16)
project(RegisterScanPlugin)

set(CMAKE_CXX_STANDARD 17)

set(CPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

add_subdirectory(${CPP_ROOT} oVnSensor)

add_library(${PROJECT_NAME} STATIC RegisterScan.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CPP_ROOT}/libs)
target_include_directories(${PROJECT_NAME} PUBLIC oVnSensor)
target_link_libraries(${PROJECT_NAME} oVnSensor)
message(STATUS "Built ${PROJECT_NAME}")
