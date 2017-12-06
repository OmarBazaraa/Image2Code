#pragma once

//
// PATHS
//
#define INPUT_IMG               "Data\\Input\\set4.jpg"
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

//
// LINE SEGMENTATION
//
#define CHUNKS_NUMBER           20
#define TEST_LINE_COLOR         Vec3b(255, 0, 255) // Magenta color.

//
// CHAR SEGMENTATION
//
#define THRESHOLD               200