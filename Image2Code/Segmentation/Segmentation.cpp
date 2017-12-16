#include "Segmentation.h"

//
// Static members declaration
//
cv::Mat Segmentation::colorImg;
cv::Mat Segmentation::binaryImg;
cv::Mat Segmentation::segmentedImg;
cv::Mat Segmentation::redImg;
string Segmentation::code;
int Segmentation::avgCharWidth;
//========================================================================


void Segmentation::segment(cv::Mat& img) {
	// Set variables
	colorImg = img;
	code = "";
	avgCharWidth = 0;

	// Preprocessing
	Segmentation::preprocess();
	Segmentation::skewCorrection();
	cv::cvtColor(binaryImg, segmentedImg, cv::COLOR_GRAY2BGR);

	// Save preprocessed image
	imwrite(PREPROCESSED_IMG, binaryImg);

	// Line segmentation
	vector<cv::Mat> lines;
	LineSegmentation::segment(binaryImg, lines);

	for (int i = 0; i < lines.size(); ++i) {
		processLine(lines[i], Utilities::toString(i, 2));
	}

	// Save segmented image
	imwrite(SEGMENTED_IMG, segmentedImg);
}

void Segmentation::processLine(cv::Mat& lineImg, const string& imgName) {
	// Save line image
	imwrite(LINE_OUTPUT_PATH + imgName + ".jpg", lineImg);

	// Word segmentation
	vector<cv::Mat> words;
	avgCharWidth = WordSegmentation::segment(lineImg, words);

	for (int i = 0; i < words.size(); ++i) {
		processWord(words[i], imgName + Utilities::toString(i, 2));
	}

	code += "\n";
}

void Segmentation::processWord(cv::Mat& wordImg, const string& imgName) {
	// Save word image
	imwrite(WORD_OUTPUT_PATH + imgName + ".jpg", wordImg);

	// Get the corresponding word from the original قثي image
	cv::Size size;
	cv::Point offset;
	wordImg.locateROI(size, offset);
	cv::Mat redWordImg = redImg(cv::Rect(offset.x, offset.y, wordImg.cols, wordImg.rows)).clone();

	// Character segmentation
	vector<cv::Mat> chars;
	CharSegmentation::segment(wordImg, redWordImg, chars, avgCharWidth);

	for (int i = 0; i < chars.size(); ++i) {
		int x = offset.x + CharSegmentation::regions[i].L;
		int y = offset.y + CharSegmentation::regions[i].U;
		processChar(chars[i], imgName + Utilities::toString(i, 2), x, y, CharSegmentation::regions[i].type);
	}

	// Draw rectangle around the word and its chars
	Utilities::drawRect(segmentedImg, offset.x, offset.y, wordImg.cols, wordImg.rows, WORD_RECT_COLOR);
	Utilities::drawRect(segmentedImg, offset.x - 1, offset.y - 1, wordImg.cols + 2, wordImg.rows + 2, WORD_RECT_COLOR);

	//
	code += " ";
}

void Segmentation::processChar(cv::Mat& charImg, const string& imgName, int offsetX, int offsetY, bool special) {
	// Save character image
	string path = CHARACTER_OUTPUT_PATH + imgName;
	if (special) {
		path += 'x';
	}
	imwrite(path + ".jpg", charImg);

	// Choose color
	cv::Vec3b color = (special ? SPECIAL_CHAR_RECT_COLOR : CHAR_RECT_COLOR);
	
	// Draw rectanlge around the characte
	Utilities::drawRect(segmentedImg, offsetX, offsetY, charImg.cols, charImg.rows, color);
	Utilities::drawRect(segmentedImg, offsetX - 1, offsetY - 1, charImg.cols + 2, charImg.rows + 2, color);

	// Character classification
	code += '*';
}

void Segmentation::preprocess() {
	// Resize the image
	//Utilities::resizeImage(rgbImg, IMG_MAX_WIDTH, IMG_MAX_HEIGHT);

	// Convert image to grayscale
	cv::cvtColor(colorImg, binaryImg, cv::COLOR_BGR2GRAY);

	// Apply noise reduction Gaussian filter
	//cv::GaussianBlur(binaryImg, binaryImg, cv::Size(3, 3), 3, 3);
	//cv::imwrite(BLURED_IMG, binaryImg);

	// Apply Otsu thresholding
	cv::threshold(binaryImg, binaryImg, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	cv::imwrite(THRESH_IMG, binaryImg);

	// Threshold the HSV image to keep only the red pixels needed for special chars detection
	cv::Mat hsvImg, lowerRedImg, upperRedImg;
	cv::cvtColor(colorImg, hsvImg, cv::COLOR_BGR2HSV);
	cv::inRange(hsvImg, cv::Scalar(0, 50, 50), cv::Scalar(15, 255, 255), lowerRedImg);
	cv::inRange(hsvImg, cv::Scalar(150, 50, 50), cv::Scalar(179, 255, 255), upperRedImg);
	cv::addWeighted(lowerRedImg, 1.0, upperRedImg, 1.0, 0.0, redImg);
	cv::imwrite(RED_THRESH_IMG, redImg);
}

void Segmentation::skewCorrection() {
	cv::Mat img = binaryImg.clone();

	// Push all foreground points
	vector<cv::Point> points;
	for (auto it = img.begin<uchar>(); it != img.end<uchar>(); ++it) {
		if (*it == FORECOLOR) {
			points.push_back(it.pos());
		}
	}

	// Get the bounding rectangle with the minimum area around the foreground pixels
	cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));

	// Draw the bounding rectangle (for visualization purpose)
	cv::Point2f vertices[4];
	box.points(vertices);
	for (int i = 0; i < 4; ++i) {
		cv::line(img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), 1, CV_AA);
	}
	cv::imwrite(BOUNDING_BOX_IMG, img);

	// Calculate the skewness angle and get the corresponding rotation matrix
	double angle = box.angle;
	if (angle < -45.0) {
		angle += 90.0;
	}
	cout << "Skew angle = " << angle << endl;
	cv::Mat rotationMat = cv::getRotationMatrix2D(box.center, angle, 1);

	// Rotate the images by the calculated angle
	cv::warpAffine(binaryImg, binaryImg, rotationMat, binaryImg.size(), cv::INTER_CUBIC);
	cv::warpAffine(redImg, redImg, rotationMat, redImg.size(), cv::INTER_CUBIC);

	cv::threshold(binaryImg, binaryImg, 70, 255, CV_THRESH_BINARY);
	cv::threshold(redImg, redImg, 70, 255, CV_THRESH_BINARY);

	cv::imwrite(RED_ROTATED_IMG, redImg);
}