// STL libraries
#include <iostream>
#include <vector>
#include <ctime>

// Custom libraries
#include "Utilities\Constants.h"
#include "Utilities\Utilities.h"
#include "Segmentation\Segmentation.h"

using namespace std;
using namespace cv;


// Main program driver function
int main() {
	int startTime = clock();

	try {
		// Clear previous output directory
		Utilities::removeDir(OUTPUT_PATH);
		Utilities::makeDir(PREPROCESS_OUTPUT_PATH);
		Utilities::makeDir(LINE_OUTPUT_PATH);
		Utilities::makeDir(WORD_OUTPUT_PATH);
		Utilities::makeDir(CHARACTER_OUTPUT_PATH);

		// Load image
		cout << "Loading image " << INPUT_IMG << "..." << endl;
		cv::Mat img = Utilities::loadImage(INPUT_IMG);

		// Segmentation
		cout << "Segmenting image..." << endl;
		Segmentation::segment(img);
				
		// Print code
		cout << Segmentation::code << endl;
	}
	catch (const exception& ex) {
		cout << "ERROR::" << ex.what() << endl;
	}

	// Output process time
	int stopTime = clock();
	cout << "Time: " << (stopTime - startTime) / double(CLOCKS_PER_SEC) << "sec" << endl;
	return 0;
}