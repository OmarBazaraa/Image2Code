#pragma once

//
// PATHS
//
#define INPUT_IMG               "Data\\Input\\extreme_test.jpg"
#define PREPROCESSED_IMG        "Data\\Output\\preprocessed.jpg"
#define OUTPUT_PATH             "Data\\Output"
#define LINE_OUTPUT_PATH        "Data\\Output\\Lines\\"
#define WORD_OUTPUT_PATH        "Data\\Output\\Words\\"
#define CHARACTER_OUTPUT_PATH   "Data\\Output\\Characters\\"
#define CODE_OUTPUT_PATH        "Data\\code.cpp"

//
// GLOBAL CONSTANTS
//
#define BACKCOLOR               255
#define FORECOLOR               0

#define IMG_MAX_WIDTH			1000
#define IMG_MAX_HEIGHT			1500

//
// LINE SEGMENTATION
//
#define CHUNKS_NUMBER           20
#define TEST_LINE_COLOR         Vec3b(255, 0, 255) // Magenta color.

//
// CHAR SEGMENTATION
//
#define THRESHOLD               200