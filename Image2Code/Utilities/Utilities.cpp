#include "Utilities.h"

cv::Mat Utilities::loadImage(const string& path) {
	// Load grayscale image from file
	Mat img = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	// Check for invalid input
	if (img.empty() || !img.data) {
		cout << path;
		string errorMessage = "Could not load the image at: " + path;
		throw exception();
	}

	return img;
}

// Testing
void adaptiveOtsuThreshold(const cv::Mat& src, cv::Mat& dst, int blockSize) {
	int rows = src.rows;
	int cols = src.cols;
	
	for (int i = 0; i < rows; i += blockSize) {
		for (int j = 0; j < cols; j += blockSize) {
			int w = (j + blockSize <= cols) ? blockSize : cols - j;
			int h = (i + blockSize <= rows) ? blockSize : rows - i;

			cv::threshold(
				src(Rect(j, i, w, h)),
				dst(Rect(j, i, w, h)),
				0,
				255,
				CV_THRESH_BINARY | CV_THRESH_OTSU
			);
		}
	}
}

// Testing
void customThreshold(const cv::Mat& src, cv::Mat& dst, int blockSize) {
	int rows = src.rows;
	int cols = src.cols;

	int hist[256];
	memset(hist, 0, sizeof(hist));

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int val = src.at<uchar>(i, j);

			hist[val]++;
		}
	}

	for (int i = 1; i < 256; ++i) {
		hist[i] += hist[i - 1];
	}

	int thresholdVal;

	for (int i = 0; i < 256; ++i) {
		if (hist[i] * 15 >= hist[255]) {
			thresholdVal = i;
			break;
		}
	}

	cv::threshold(src, dst, thresholdVal, 255, cv::THRESH_BINARY);
}

// Testing
void gammaCorrection(const Mat& src, cv::Mat& dst, double gamma) {
	cv::Mat lookUpTable(1, 256, CV_8U);

	uchar* p = lookUpTable.ptr();

	for (int i = 0; i < 256; ++i) {
		p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
	}

	cv::LUT(src, lookUpTable, dst);
}

cv::Mat Utilities::preprocess(const cv::Mat& img) {
	// Apply noise reduction Gaussian filter
	cv::Mat smoothedImg;
	GaussianBlur(img, smoothedImg, Size(5, 5), 3, 3);
	imwrite("Data\\0.blured.png", smoothedImg);

	// Fix image brightness
	Mat gammaImg;
	gammaCorrection(smoothedImg, gammaImg, 0.5);
	imwrite("Data\\1.gamma.png", gammaImg);
	
	// Apply basic thresholding
	cv::Mat binaryImg;
	//cv::threshold(smoothedImg, binaryImg, THRESHOLD, 255, cv::THRESH_BINARY);
	cv::threshold(gammaImg, binaryImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imwrite("Data\\2.thresholded.png", binaryImg);
	
	// Connect characters
	cv::Mat dilatedImg, erodedImg;
	cv::Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	cv::dilate(binaryImg, dilatedImg, element);
	cv::erode(dilatedImg, erodedImg, element);
	imwrite("Data\\3.dilated.png", dilatedImg);
	imwrite("Data\\4.eroded.png", erodedImg);
	
	return erodedImg;

	//// Apply adaptive Gaussian thresholding
	//cv::Mat binaryImg(img.size(), img.type());
	//cv::adaptiveThreshold(img, binaryImg, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 25, 0.5);
	//
	//imwrite("Data\\thresholded.png", binaryImg);
	//
	//// Apply median filter to remove impulsive noise
	//cv::Mat filterdImg;
	//cv::medianBlur(binaryImg, filterdImg, 3);
	//
	//imwrite("Data\\filterd.png", filterdImg);
	//
	//// Connect characters
	//cv::Mat dilatedImg, erodedImg;
	//cv::Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	//cv::erode(filterdImg, erodedImg, element);
	//cv::dilate(erodedImg, dilatedImg, element);
	//
	//imwrite("Data\\eroded.png", binaryImg);
	//imwrite("Data\\dilated.png", binaryImg);
	//
	//return dilatedImg;
}

string Utilities::postprocess(const string& str) {
	return str;
}

void Utilities::makeDir(const string& dir) {
	string cmd = "mkdir " + dir;
	system(cmd.c_str());
}

void Utilities::removeDir(const string& dir) {
#ifdef _WIN32
	string cmd = "rmdir " + dir + " /S/Q";
#else
	string cmd = "rm -r " + dir;
#endif

	system(cmd.c_str());
}