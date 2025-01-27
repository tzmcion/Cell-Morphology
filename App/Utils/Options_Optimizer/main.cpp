#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Threading/Threads.h"
#include "../../Components/Structures/AlgorithmOptions.h"

/**
 * This file aims to optimize the options for Full segmentation_ being
 * - Background Extractor
 * - Foreground regions extractor
 * - Foreground markers extractor
 * This file outputs the .option file for algorithms, template of this file is in /Utils/algorithm_efault.options
 * This file works in a fast way, searching for best matching from default for every step iteratively, so
 * Looking for most optimized in 1st argument, then in 2nd argument etc...
 * May not always work the best
 */

/**
 * Program idea - extract small part of the image and ask user to fill the centers of the cells
 * Try to match the parameters of the programm to best fit with the centers of the cells.
 */

/*
Searching idea in this example will be covered with binary search.
Each parameter will go minus and plus in a diverse ways, iterating over max 50 attempts to find the best fit for the image.
Will se how it goes
*/

int main(int argc, char **argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 4){
        throw std::invalid_argument("Number of argumnets is invalid, required is 3");
    }
    const char* OPTIONS_PATH = std::string(argv[1]) == "def" ? "../optimizer_temp.option" : argv[1];
    const char* PATHS = argv[2];
    const char* OUT_FOLDER = argv[3];
    const std::string INP_DATA = Entites::Convert::text_file_to_string("../Options_Optimizer/README.md");

    //REST TODO I GO TO SLEEP
}