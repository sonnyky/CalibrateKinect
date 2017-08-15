# CalibrateKinect
## Dependencies
* CMake, v2.8 or above
* Kinect SDKv2.0
* OpenCV v3.1.0
* Visual Studio. Tested on Visual Studio Community 2017.

## Usage
* Clone project to a folder. We will call this ${ROOT}.
* Run CMake GUI and specify src folder as ${ROOT}/src. Folder to build the binaries will be ${ROOT}/build.
* Press configure and then press Generate
* Look inside ${ROOT}/build and CMake would have generated a Visual Studio solution, calibrate_kinect.solution.
* Open the solution and build. The binaries will go into ${ROOT}/build/Debug.
