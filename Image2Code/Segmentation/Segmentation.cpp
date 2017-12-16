#include "Segmentation.h"

//
// Static members declaration
//
cv::Mat Segmentation::rgbImg;
cv::Mat Segmentation::binaryImg;
cv::Mat Segmentation::segmentedImg;
cv::Mat Segmentation::redImg;
string Segmentation::code;
int Segmentation::avgCharWidth;
//========================================================================


void Segmentation::segment(cv::Mat& img) {
	// Set variables
	rgbImg = img;
	code = "";
	avgCharWidth = 0;

	// Preprocessing
	Segmentation::preprocess();
	cv::cvtColor(binaryImg, segmentedImg, cv::COLOR_GRAY2BGR);

	// Line segmentation
	vector<cv::Mat> lines;
	LineSegmentation::segment(binaryImg, lines);

	for (int i = 0; i < lines.size(); ++i) {
		processLine(lines[i], to_string(i));
	}

	// Postprocessing
	postprocess();

	// Save preprocessed & segmented image
	imwrite(PREPROCESSED_IMG, binaryImg);
	imwrite(SEGMENTED_IMG, segmentedImg);
}

void Segmentation::processLine(cv::Mat& lineImg, const string& imgName) {
	// Save line image
	imwrite(LINE_OUTPUT_PATH + imgName + ".jpg", lineImg);

	// Word segmentation
	vector<cv::Mat> words;
	avgCharWidth = WordSegmentation::segment(lineImg, words);

	for (int i = 0; i < words.size(); ++i) {
		processWord(words[i], imgName + "_" + to_string(i));
	}

	code += "\n";
}

void Segmentation::processWord(cv::Mat& wordImg, const string& imgName) {
	// Save word image
	imwrite(WORD_OUTPUT_PATH + imgName + ".jpg", wordImg);

	// Get the corresponding word from the original RGB image
	cv::Size size;
	cv::Point offset;
	wordImg.locateROI(size, offset);
	cv::Mat rgbWordImg = redImg(cv::Rect(offset.x, offset.y, wordImg.cols, wordImg.rows)).clone();

	// Character segmentation
	vector<cv::Mat> chars;
	CharSegmentation::segment(wordImg, rgbWordImg, chars, avgCharWidth);

	for (int i = 0; i < chars.size(); ++i) {
		processChar(chars[i], imgName + "_" + to_string(i));
	}

	// Draw rectangle around the word and its chars
	highlightWord(wordImg);

	code += " ";
}

void Segmentation::processChar(cv::Mat& charImg, const string& imgName) {
	// Save character image
	imwrite(CHARACTER_OUTPUT_PATH + imgName + ".jpg", charImg);

	// Character classification
	code += '*'; //Classifier::classify(chars[i]);
}

void Segmentation::highlightWord(cv::Mat& wordImg) {
	int x, y, w, h;
	cv::Size size;
	cv::Point offset;

	// Get word offset inside the image
	wordImg.locateROI(size, offset);

	// Draw green rectangles around each character
	for (auto& r : CharSegmentation::regions) {
		x = offset.x + r.L;
		y = offset.y + r.U;
		w = r.R - r.L + 1;
		h = r.D - r.U + 1;

		if (r.type) {
			Utilities::drawRect(segmentedImg, x, y, w, h, SPECIAL_CHAR_RECT_COLOR);
			Utilities::drawRect(segmentedImg, x - 1, y - 1, w + 2, h + 2, SPECIAL_CHAR_RECT_COLOR);
		}
		else {
			Utilities::drawRect(segmentedImg, x, y, w, h, CHAR_RECT_COLOR);
			Utilities::drawRect(segmentedImg, x - 1, y - 1, w + 2, h + 2, CHAR_RECT_COLOR);
		}
	}

	// Draw magenta rectangle around the whole word
	x = offset.x;
	y = offset.y;
	w = wordImg.cols;
	h = wordImg.rows;
	Utilities::drawRect(segmentedImg, x, y, w, h, WORD_RECT_COLOR);
	Utilities::drawRect(segmentedImg, x - 1, y - 1, w + 2, h + 2, WORD_RECT_COLOR);
}

void Segmentation::preprocess() {
	// Threshold the HSV image to keep only the red pixels needed for special chars detection
	cv::Mat hsvImg, lowerRedImg, upperRedImg;
	cv::cvtColor(rgbImg, hsvImg, cv::COLOR_BGR2HSV);
	cv::inRange(hsvImg, cv::Scalar(0, 50, 50), cv::Scalar(15, 255, 255), lowerRedImg);
	cv::inRange(hsvImg, cv::Scalar(150, 50, 50), cv::Scalar(179, 255, 255), upperRedImg);
	cv::addWeighted(lowerRedImg, 1.0, upperRedImg, 1.0, 0.0, redImg);
	cv::imwrite("Data\\0.red.png", redImg);

	// Resize the image
	//Utilities::resizeImage(rgbImg, IMG_MAX_WIDTH, IMG_MAX_HEIGHT);

	// Convert image to grayscale
	cv::cvtColor(rgbImg, binaryImg, cv::COLOR_RGB2GRAY);

	// Apply noise reduction Gaussian filter
	cv::GaussianBlur(binaryImg, binaryImg, cv::Size(5, 5), 3, 3);
	cv::imwrite("Data\\0.blured.png", binaryImg);

	// Fix image brightness
	//Utilities::gammaCorrection(binaryImg, 0.5);
	//cv::imwrite("Data\\1.gamma.png", binaryImg);

	// Apply Otsu thresholding
	cv::threshold(binaryImg, binaryImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imwrite("Data\\2.thresholded.png", binaryImg);

	// Connect characters
	cv::Mat tmp;
	cv::Mat element = cv::getStructuringElement(MORPH_CROSS, Size(3, 3));
	cv::dilate(binaryImg, tmp, element, cv::Point(-1, -1), 1);
	cv::imwrite("Data\\3.dilated.png", tmp);
	cv::erode(tmp, binaryImg, element, cv::Point(-1, -1), 1);
	cv::imwrite("Data\\4.eroded.png", binaryImg);
}

void Segmentation::postprocess() {

	
}