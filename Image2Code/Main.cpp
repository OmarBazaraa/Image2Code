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


// Global variables
string str;
int avgCharWidth;

// Function prototypes
void segment(cv::Mat& img);
void processLine(cv::Mat& lineImg, const string& imgName);
void processWord(cv::Mat& wordImg, const string& imgName);
void processChar(cv::Mat& charImg, const string& imgName);


// Main program driver function
int main() {
	int startTime = clock();

	try {
		// Clear previous output directory
		Utilities::removeDir(OUTPUT_PATH);
		Utilities::makeDir(LINE_OUTPUT_PATH);
		Utilities::makeDir(WORD_OUTPUT_PATH);
		Utilities::makeDir(CHARACTER_OUTPUT_PATH);

		// Load image
		cout << "Loading image " << INPUT_IMG << "..." << endl;
		cv::Mat img = Utilities::loadImage(INPUT_IMG);

		// Preprocessing
		cout << "Preprocessing..." << endl;
		cv::Mat binaryImg = Utilities::preprocess(img);
		
		// Segmentation
		cout << "Segmenting image..." << endl;
		segment(binaryImg);

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

//
void segment(cv::Mat& img) {
	// Save preprocessed image
	imwrite(PREPROCESSED_IMG, img);

	// Clear variables
	str = "";
	avgCharWidth = 0;
	
	// Line segmentation
	vector<cv::Mat> lines;
	LineSegmentation::segment(img, lines);

	for (int i = 0; i < lines.size(); ++i) {
		processLine(lines[i], to_string(i));
	}
}

//
void processLine(cv::Mat& lineImg, const string& imgName) {
	// Save line image
	imwrite(LINE_OUTPUT_PATH + imgName + ".jpg", lineImg);

	// Word segmentation
	vector<cv::Mat> words;
	avgCharWidth = WordSegmentation::segment(lineImg, words);
	
	for (int i = 0; i < words.size(); ++i) {
		processWord(words[i], imgName + "_" + to_string(i));
	}

	str += "\n";
}

//
void processWord(cv::Mat& wordImg, const string& imgName) {
	// Save word image
	imwrite(WORD_OUTPUT_PATH + imgName + ".jpg", wordImg);

	// Character segmentation
	vector<cv::Mat> chars;
	CharSegmentor::segment(wordImg, chars, avgCharWidth);

	for (int i = 0; i < chars.size(); ++i) {
		processChar(chars[i], imgName + "_" + to_string(i));
	}

	str += " ";
}

//
void processChar(cv::Mat& charImg, const string& imgName) {
	// Save character image
	imwrite(CHARACTER_OUTPUT_PATH + imgName + ".jpg", charImg);

	// Character classification
	str += '*'; //Classifier::classify(chars[i]);
}