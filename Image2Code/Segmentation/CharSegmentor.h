#pragma once
// STL libraries
#include <iostream>
#include <vector>
#include <map>
using namespace std;

// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

// Constants
const int BACKCOLOR = 255;
const int FORECOLOR = 0;
const int MERGE_THRESHOLD = 50;
const int dirR[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dirC[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

struct Region {
	int U;	// Up
	int D;	// Down
	int L;	// Left
	int R;	// Right
	int id;

	Region() {
		U = L = 1e9;
		D = R = -1e9;
	}

	void merge(const Region& rhs) {
		U = min(U, rhs.U);
		D = max(D, rhs.D);
		L = min(L, rhs.L);
		R = max(R, rhs.R);
	}

	bool operator<(const Region& rhs) const {
		if (R == rhs.R)
			return L > rhs.L;
		return R < rhs.R;
	}
};

class CharSegmentor {
private:
	static int n, m, id;
	static cv::Mat word;
	static cv::Mat visited;
	static vector<Region> regions;
	static map<int, int> regionsID;
	static Region region;

public:
	static void segment(cv::Mat& img, vector<cv::Mat>& chars);

private:
	CharSegmentor() {}	// Private constructor

	static void extractChars(vector<cv::Mat>& chars);

	static void mergeRegions();

	static void dfs(int row, int col);

	static bool valid(int row, int col);

	static void init(cv::Mat& img);
};