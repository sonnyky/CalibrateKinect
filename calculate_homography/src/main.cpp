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

		if (argc == 9) {
			capture.setRangePoints(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]));
		}
		else {
			cout << "No input provided for range points" << endl;
		}

		capture.run();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

	return 0;
}