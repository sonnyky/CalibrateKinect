# CalibrateKinect
This is a tool to calibrate the Kinect V2 using OpenCV.
## Dependencies
This project currently uses the following.
* CMake, v2.8 or above
* Kinect SDKv2.0
* OpenCV v3.1.0
* Visual Studio. Tested on Visual Studio Community 2017.

## Installation and Building
* Clone project to a folder. We will call this ${ROOT}.
* Run CMake GUI and specify src folder as ${ROOT}. Folder to build the binaries will be ${ROOT}/build.
* Press configure and then press Generate
* Look inside ${ROOT}/build and CMake would have generated a Visual Studio solution called calibrate_kinect.solution.
* Open the solution and build. The binaries will go into ${ROOT}/build/Debug or Release depending on the build configuration.

## Calibration Functions
### Calculating Camera Parameters
* Print the calibration pattern located in ${ROOT}/images.
* Run the exe generated in ${ROOT}/build/Debug or Release depending on the build configuration. The calibration won't start until you press 'c'.
* Every time the key 'c' is pressed, it captures one image and waits for the next one. The number of images to capture before calibration runs is currently set at 10.
* After capturing 10 images, a camera_parameters.yml file will be generated and the application will exit.
* Press 'q' anytime when the application is running to quit application.
### Calculating Homography Matrix
* Launch calculate_homography.exe with 4 points as parameters. These 4 points are the destination points.
* The parameters must be in the following order : Top Left, Top Right, Bottom Left, Bottom Right.
* Left click anywhere on the image to determine __Top Left__ point.
* Shift + Left click to determine __Top Right__ point.
* Right click to determine __Bottom Left__ point.
* Shift + Right click to determine __Bottom Right__ point.
* The order when clicking does not matter, and you can redo clicking as many times as you want.
* Press __c__ to calculate homography matrix. A file called homography.xml will be generated in the build folder. (in the same folder as the exe file)
* Press __q__ to quit the application.
