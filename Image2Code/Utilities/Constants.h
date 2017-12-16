#pragma once

//
// PATHS
//
#define INPUT_IMG               "Data\\Input\\sample_4.jpg"
#define PREPROCESSED_IMG        "Data\\Output\\preprocessed.jpg"
#define SEGMENTED_IMG           "Data\\Output\\segmented.jpg"
#define OUTPUT_PATH             "Data\\Output"
#define LINE_OUTPUT_PATH        "Data\\Output\\0.Lines\\"
#define WORD_OUTPUT_PATH        "Data\\Output\\1.Words\\"
#define CHARACTER_OUTPUT_PATH   "Data\\Output\\2.Chars\\"
#define CODE_OUTPUT_PATH        "Data\\code.cpp"

//
// GLOBAL CONSTANTS
//
#define BACKCOLOR               255
#define FORECOLOR               0
#define WORD_RECT_COLOR         Vec3b(255, 0, 255) // Magenta color.
#define CHAR_RECT_COLOR			Vec3b(0, 255, 0)
#define SPECIAL_CHAR_RECT_COLOR	Vec3b(3, 158, 233)

#define IMG_MAX_WIDTH           1000
#define IMG_MAX_HEIGHT          1500