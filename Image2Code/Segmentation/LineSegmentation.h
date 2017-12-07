#pragma once

// STL libraries
#include <vector>

// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Custom libraries
#include "../Utilities/Constants.h"

using namespace std;
using namespace cv;


class LineSegmentation {
private:
	static int rows, cols, L, R;
	static cv::Mat img;
	static vector<int> pixelsCount;
	static vector<pair<int, int>> blankLines;

public:
	/**
	 * Segment the given text image into lines.
	 */
	static void segment(cv::Mat& img, vector<cv::Mat>& lines);

private:
	/**
	 * Private constructor (i.e. static class).
	 */
	LineSegmentation() {}

	/**
	 * Detect the white spaces between the lines in the given text image.
	 */
	static void detectBlankLines(int threshold);

	/**
	* Divide the given line into words according to the white spaces.
	*/
	static void divideImage(vector<cv::Mat>& lines);

	/**
	 * Initialize word segmentation variables.
	 */
	static void init(cv::Mat& img);
};