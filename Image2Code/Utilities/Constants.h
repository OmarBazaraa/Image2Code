#pragma once

//
// PATHS
//
#define INPUT_IMG               "Data\\Input\\sample_0.jpg"

#define OUTPUT_PATH             "Data\\Output"
#define PREPROCESS_OUTPUT_PATH  "Data\\Output\\0.Preprocess\\"
#define LINE_OUTPUT_PATH        "Data\\Output\\1.Lines\\"
#define WORD_OUTPUT_PATH        "Data\\Output\\2.Words\\"
#define CHARACTER_OUTPUT_PATH   "Data\\Output\\3.Chars\\"

#define THRESH_IMG              "Data\\Output\\0.Preprocess\\0.thresholded.jpg"
#define BOUNDING_BOX_IMG        "Data\\Output\\0.Preprocess\\1.bounding_box.jpg"
#define ROTATED_IMG             "Data\\Output\\0.Preprocess\\2.rotated.jpg"
#define RED_THRESH_IMG          "Data\\Output\\0.Preprocess\\3.red.jpg"
#define SEGMENTED_IMG           "Data\\Output\\0.Preprocess\\5.segmented.jpg"

//
// GLOBAL CONSTANTS
//
#define BACKCOLOR               0
#define FORECOLOR               255
#define WORD_RECT_COLOR         Vec3b(255, 0, 255) // Magenta color.
#define CHAR_RECT_COLOR			Vec3b(0, 255, 0)
#define SPECIAL_CHAR_RECT_COLOR	Vec3b(232, 162, 0)

#define IMG_MAX_WIDTH           1000
#define IMG_MAX_HEIGHT          1500