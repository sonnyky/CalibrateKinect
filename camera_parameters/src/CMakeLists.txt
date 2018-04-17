# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define project name
project(camera_parameters)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
set( OpenCV_DIR "D:/Libraries/opencv3.1.0/build" )
option( OpenCV_STATIC OFF )
find_package(OpenCV REQUIRED)

# Kinect
set(KinectSDK20_ROOT         $ENV{KINECTSDK20_DIR})
set(KinectSDK20_LIB_DIR      ${KinectSDK20_ROOT}/Lib/x86) # THIS SHOULD BE AUTOMATIC TO CHOOSE BETWEEN x64 and x86
set(KinectSDK20_INCLUDE_DIRS ${KinectSDK20_ROOT}/inc)
set(KinectSDK20_LIBRARIES    ${KinectSDK20_LIB_DIR}/Kinect20.lib)

# Set installation folder
set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )

# Set additional include folders
set (ADDITIONAL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories(${ADDITIONAL_INCLUDE})
include_directories(${KinectSDK20_INCLUDE_DIRS})
link_directories(${KinectSDK20_LIB_DIR})

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
  # Add OpenCV headers location to your include paths
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# Declare the executable target built from your sources
set(SOURCES main.cpp app.cpp ${ADDITIONAL_INCLUDE}/app.h ${ADDITIONAL_INCLUDE}/util.h)
add_executable(camera_parameters ${SOURCES} )

# Link your application with OpenCV libraries
target_link_libraries(camera_parameters ${OpenCV_LIBS} ${KinectSDK20_LIBRARIES})