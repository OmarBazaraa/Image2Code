#pragma once
#include "CharSegmentor.h"

//
// Static members declaration
//
int CharSegmentor::rows;
int CharSegmentor::cols;
int CharSegmentor::id;
cv::Mat CharSegmentor::word;
cv::Mat CharSegmentor::visited;
vector<Region> CharSegmentor::regions;
map<int, int> CharSegmentor::regionsID;
Region CharSegmentor::region;
//========================================================================


void CharSegmentor::segment(cv::Mat& img, vector<cv::Mat>& chars) {
	// Initialization
	init(img);

	// Scan word matrix
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			// Continue if previously visited or background pixel
			if (visited.at<uchar>(i, j) || word.at<uchar>(i, j) == BACKCOLOR) {
				continue;
			}

			// Get region boundries	
			region = Region();
			region.id = ++id;
			regionsID[id] = id;
			dfs(i, j);

			int w = region.R - region.L + 1;
			int h = region.D - region.U + 1;

			// Ignore component if it is smaller than a certain threshold
			if (min(w, h) <= IGNORE_SIZE_THRESHOLD) {
				continue;
			}

			// Store character region
			regions.push_back(region);
		}
	}

	//preprocessRegions();
	mergeRegions();
	extractChars(chars);
}

void CharSegmentor::extractChars(vector<cv::Mat>& chars) {
	for (auto& r : regions) {
		int w = r.R - r.L + 1;
		int h = r.D - r.U + 1;
		cv::Mat m(h, w, CV_8U, cv::Scalar(BACKCOLOR));

		for (int i = r.U; i <= r.D; ++i) {
			for (int j = r.L; j <= r.R; ++j) {
				id = visited.at<uchar>(i, j);

				if (regionsID[id] == r.id) {
					m.at<uchar>(i - r.U, j - r.L) = FORECOLOR;
				}
			}
		}

		chars.push_back(m);
	}
}

void CharSegmentor::mergeRegions() {
	vector<Region> tmp;
	vector<bool> vis(regions.size(), 0);
	sort(regions.begin(), regions.end());

	for (int i = 0; i < regions.size(); ++i) {
		if (vis[i]) {
			continue;
		}

		tmp.push_back(regions[i]);
		Region& p = tmp.back();

		for (int j = i + 1; j < regions.size(); ++j) {
			Region& q = regions[j];

			if (q.L > p.R) {
				break;
			}
			
			int commonWidth = p.R - max(p.L, q.L) + 1;
			int width = min(p.R - q.L, q.R - q.L) + 1;

			if (commonWidth * 100 >= width * MERGE_X_THRESHOLD) {
				vis[j] = 1;
				regionsID[q.id] = p.id;
				p.merge(q);
			}
		}
	}

	regions.swap(tmp);
}

void CharSegmentor::dfs(int row, int col) {
	// Update boundries
	region.U = min(region.U, row);
	region.L = min(region.L, col);
	region.D = max(region.D, row);
	region.R = max(region.R, col);

	// Set current pixel as visisted with current component id
	visited.at<uchar>(row, col) = id;

	// Visit neighbours
	for (int i = 0; i < 8; ++i) {
		int toR = row + dirR[i];
		int toC = col + dirC[i];

		if (valid(toR, toC) && !visited.at<uchar>(toR, toC)) {
			dfs(toR, toC);
		}
	}
}

bool CharSegmentor::valid(int row, int col) {
	return (
		row >= 0 && row < rows &&
		col >= 0 && col < cols &&
		word.at<uchar>(row, col) == FORECOLOR
	);
}

void CharSegmentor::init(cv::Mat& img) {
	rows = img.rows;
	cols = img.cols;
	id = 0;
	word = img;
	visited = cv::Mat::zeros(rows, cols, CV_8U);
	regions.clear();
	regionsID.clear();
}