#include "LineSegmentation.h"

void LineSegmentation::segment(const cv::Mat& img, vector<cv::Mat>& lines) {
	int spaceThreshold = 5; // TODO: to be calculated dynamically
	divideImage(img, lines, spaceThreshold);
}

void LineSegmentation::divideImage(const cv::Mat& img, vector<cv::Mat>& lines, int threshold) {
	int rows = img.rows;
	int cols = img.cols;
	vector<int> pixelsCount(rows, 0);

	// Calculate black pixels count in each row.
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			pixelsCount[i] += (img.at<uchar>(i, j) == FORECOLOR);
		}
	}

	int l = 0;
	int r = rows - 1;
	int cnt = 0;
	vector<pair<int, int>> blankLines;

	// Skip top blank rows
	while (l < rows && pixelsCount[l] == 0) {
		l++;
	}
	blankLines.push_back({ 0, l - 1 });

	// Skip bottom blank rows
	while (r > l && pixelsCount[r] == 0) {
		r--;
	}

	// Detect white spaces
	for (int i = l; i <= r; ++i) {
		if (pixelsCount[i] > 0) {
			if (cnt > threshold) {
				blankLines.push_back({ l, i - 1 });
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
	blankLines.push_back({ r + 1, rows - 1 });

	// Detect words between two successive white spaces
	for (int i = 1; i < blankLines.size(); ++i) {
		l = blankLines[i - 1].second + 1;
		r = blankLines[i].first - 1;

		if (r <= l) {
			continue;
		}

		Mat line = img(cv::Rect(0, l, cols, r - l + 1));
		lines.push_back(line.clone());
	}
}