#include "Utilities.h"

cv::Mat Utilities::loadImage(const string& path) {
	// Load grayscale image from file
	cv::Mat img = cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	// Check for invalid input
	if (img.empty() || !img.data) {
		cout << path;
		string errorMessage = "Could not load the image at: " + path;
		throw exception();
	}

	return img;
}

void Utilities::resizeImage(Mat& img, int maxWidth, int maxHeight) {
	if (img.rows > img.cols) {
		if (img.rows > maxHeight) {
			double ratio = maxHeight / (double)img.rows;
			cv::resize(img, img, cv::Size(0, 0), ratio, ratio);
		}
	}
	else {
		if (img.cols > maxWidth) {
			double ratio = maxWidth / (double)img.cols;
			cv::resize(img, img, cv::Size(0, 0), ratio, ratio);
		}
	}
}

void Utilities::gammaCorrection(Mat& src, double gamma) {
	cv::Mat lookUpTable(1, 256, CV_8U);

	uchar* p = lookUpTable.ptr();

	for (int i = 0; i < 256; ++i) {
		p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
	}

	cv::LUT(src, lookUpTable, src);
}

// Testing
void adaptiveOtsuThreshold(cv::Mat& src, cv::Mat& dst, int blockSize) {
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
void customThreshold(cv::Mat& src, cv::Mat& dst, int blockSize) {
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

cv::Mat Utilities::preprocess(cv::Mat& img) {
	cv::Mat resImg;

	// Resize the image
	//resizeImage(img, IMG_MAX_WIDTH, IMG_MAX_HEIGHT);

	// Apply noise reduction Gaussian filter
	cv::GaussianBlur(img, resImg, cv::Size(5, 5), 3, 3);
	cv::imwrite("Data\\0.blured.png", resImg);

	// Fix image brightness
	gammaCorrection(resImg, 0.5);
	cv::imwrite("Data\\1.gamma.png", resImg);
	
	// Apply Otsu thresholding
	cv::threshold(resImg, resImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imwrite("Data\\2.thresholded.png", resImg);
	
	// Connect characters
	cv::Mat tmp;
	cv::Mat element = cv::getStructuringElement(MORPH_CROSS, Size(3, 3));
	cv::dilate(resImg, tmp, element, cv::Point(-1, -1), 1);
	cv::imwrite("Data\\3.dilated.png", tmp);
	cv::erode(tmp, resImg, element, cv::Point(-1, -1), 1);
	cv::imwrite("Data\\4.eroded.png", resImg);
	
	return resImg;
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