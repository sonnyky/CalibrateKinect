# CalibrateKinect
## Dependencies
* CMake, v2.8 or above
* Kinect SDKv2.0
* OpenCV v3.1.0
* Visual Studio. Tested on Visual Studio Community 2017.

## Installation and Building
* Clone project to a folder. We will call this ${ROOT}.
* Run CMake GUI and specify src folder as ${ROOT}/src. Folder to build the binaries will be ${ROOT}/build.
* Press configure and then press Generate
* Look inside ${ROOT}/build and CMake would have generated a Visual Studio solution, calibrate_kinect.solution.
* Open the solution and build. The binaries will go into ${ROOT}/build/Debug.

## Calibration
* Print the calibration pattern located in ${ROOT}/images.
* Run the exe generated in ${ROOT}/build/Debug. The calibration won't start until you press 'c'.
* Every time the key 'c' is pressed, it captures one image and waits for the next one. The number of images to capture before calibration runs is currently set at 10.
* After capturing 10 images, a camera_parameters.yml file will be generated and the application will exit.
