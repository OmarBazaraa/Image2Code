#include "WordSegmentation.h"

void WordSegmentation::segment(const cv::Mat& img, vector<cv::Mat>& words) {
	int spaceThreshold = 35; // TODO: to be calculated dynamically
	divideLine(img, words, spaceThreshold);
}

void WordSegmentation::divideLine(const cv::Mat& img, vector<cv::Mat>& words, int threshold) {
	int rows = img.rows;
	int cols = img.cols;
	vector<int> x_histogram(cols, 0);
	
	// Calculate X histogram
	for (int i = 0; i < cols; ++i) {
		for (int j = 0; j < rows; ++j) {
			x_histogram[i] += (img.at<uchar>(j, i) == FORECOLOR);
		}
	}

	int l = 0;
	int r = cols - 1;
	int cnt = 0;
	vector<pair<int, int>> whiteSpaces;

	// Skip left white columns
	while (l < cols && x_histogram[l] == 0) {
		l++;
	}
	whiteSpaces.push_back({ 0, l - 1 });

	// Skip right white columns
	while (r > l && x_histogram[r] == 0) {
		r--;
	}
	
	// Detect white spaces
	for (int i = l; i <= r; ++i) {
		if (x_histogram[i] > 0) {
			if (cnt > threshold) {
				whiteSpaces.push_back({ l, i - 1 });
			}

			cnt = 0;
		}
		else {
			cnt++;

			if (cnt == 1) {
				l = i;
			}
		}
	}
	whiteSpaces.push_back({ r + 1, cols - 1 });
	
	// Detect words between two successive white spaces
	for (int i = 1; i < whiteSpaces.size(); ++i) {
		l = whiteSpaces[i - 1].second + 1;
		r = whiteSpaces[i].first - 1;

		if (r <= l) {
			continue;
		}

		Mat word = img(cv::Rect(l, 0, r - l + 1, rows));
		words.push_back(word.clone());
	}
}