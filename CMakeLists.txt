cmake_minimum_required(VERSION 2.8)
project (calibrate_kinect)
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(calculate_homography/src)
add_subdirectory(camera_parameters/src)