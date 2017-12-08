// STL libraries
#include <iostream>
#include <vector>
#include <ctime>

// Custom libraries
#include "Utilities\Constants.h"
#include "Utilities\Utilities.h"
#include "Segmentation\LineSegmentation.h"
#include "Segmentation\WordSegmentation.h"
#include "Segmentation\CharSegmentor.h"

using namespace std;
using namespace cv;


// Function prototypes
string segment(cv::Mat img);

// Main program driver function
int main() {
	int startTime = clock();

	try {
		// Clear previous output directory
		Utilities::removeDir(OUTPUT_PATH);

		// Load image
		cout << "Loading image " << INPUT_IMG << "..." << endl;
		cv::Mat img = Utilities::loadImage(INPUT_IMG);

		// Preprocessing
		cout << "Preprocessing..." << endl;
		cv::Mat binaryImg = Utilities::preprocess(img);
		
		// Segmentation
		cout << "Segmenting image..." << endl;
		string str = segment(binaryImg);

		// Postprocessing
		cout << "Postprocessing..." << endl;
		string code = Utilities::postprocess(str);

		cout << code << endl;
	}
	catch (const exception& ex) {
		cout << "ERROR::" << ex.what() << endl;
	}

	// Output process time
	int stopTime = clock();
	cout << "Time: " << (stopTime - startTime) / double(CLOCKS_PER_SEC) << "sec" << endl;
	return 0;
}

string segment(cv::Mat img) {
	string str;

	// Make output line directory
	string pathLine = LINE_OUTPUT_PATH;
	Utilities::makeDir(pathLine);
	// Save preprocessed image
	imwrite(PREPROCESSED_IMG, img);

	Utilities::makeDir("Data\\Output\\Chars\\");

	// Line segmentation
	vector<cv::Mat> lines;
	LineSegmentation::segment(img, lines);

	for (int i = 0; i < lines.size(); ++i) {
		// Make output word directory
		string pathWord = WORD_OUTPUT_PATH + to_string(i) + "\\";
		Utilities::makeDir(pathWord);
		// Save line image
		imwrite(pathLine + to_string(i) + ".jpg", lines[i]);

		// Word segmentation
		vector<cv::Mat> words;
		int avgCharWidth = WordSegmentation::segment(lines[i], words);
		
		for (int j = 0; j < words.size(); ++j) {
			// Make output character directory
			string pathChar = CHARACTER_OUTPUT_PATH + to_string(i) + "\\" + to_string(j) + "\\";
			Utilities::makeDir(pathChar);
			// Save word image
			imwrite(pathWord + to_string(j) + ".jpg", words[j]);

			// Character segmentation
			vector<cv::Mat> chars;
			CharSegmentor::segment(words[j], chars, avgCharWidth);

			for (int k = 0; k < chars.size(); ++k) {
				// Save character image
				imwrite(pathChar + to_string(k) + ".jpg", chars[k]);

				imwrite("Data\\Output\\Chars\\" + to_string(i) + "_" + to_string(j) + "_" + to_string(k) + ".jpg", chars[k]);

				// Character classification
				str += '*'; //Classifier::classify(chars[i]);
			}

			str += " ";
		}
		str += "\n";
	}

	return str;
}