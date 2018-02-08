#include <iostream>
#include <sstream>

#include "app.h"
/*
Code taken from samples provided by https://github.com/UnaNancyOwen/Kinect2Sample
*/
int main(int argc, char* argv[])
{
	try {
		Capture capture;
		capture.run();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	return 0;
}