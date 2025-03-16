#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Optimalization/Optimaliation.h"
#include "../../Components/Threading/Threads.h"

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

/**
 * The goal is to create both manual and automated options optimizers
 * So the segmentation process can be as precise as possible
 */

int main(int argc, char **argv){
    std::srand(time(NULL)); //Using random in this process
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 4){
        throw std::invalid_argument("Number of argumnets is invalid, required is 3");
    }
    const char* OPTIONS_PATH = std::string(argv[1]) == "def" ? "../optimizer_temp.option" : argv[1];
    std::string PATHS = Entites::Convert::text_file_to_string(argv[2]);
    const char* OUT_FOLDER = argv[3];
    const std::string INP_DATA = Entites::Convert::text_file_to_string("../Options_Optimizer/README.md");\
    std::vector<std::string> images;
    Entites::Convert::c_char_to_string(images,PATHS.c_str());
    for(size_t x = 0; x < images.size(); x++){
        std::cout << "Image_" << x << ' ' << Colors::MAGENTA << images[x] << Colors::RESET << std::endl;
    }
    std::cout << "Detected " << images.size() << " files to convert in total \n";
    std::cout << "Output folder is: " << Colors::YELLOW <<  OUT_FOLDER << Colors::RESET << std::endl;
    std::cout << Colors::BRIGHT_BLUE << "[INFO] " << Colors::RESET << "The output options will be saved to the output folder \n";
    //Select random image and crop/cut out 300x300 image
    Optimalization *opt = new Optimalization();
    cv::Mat cropped;
    opt->crop_save_image_sample(cropped,images,std::string(std::string(OUT_FOLDER) + std::string("/cropped_bg.jpg")));
    std::string comm_file = std::string(std::string(OUT_FOLDER) + std::string("/info.txt"));
    std::cout << comm_file;
    //Save the image 
    //The folder must exist (this "temp" folder)
    Entites::FILES::write_to_file(comm_file.c_str(),"CREATED");
    while(1){
        std::string msg = Threading::await_file_change(comm_file.c_str(),500000,2);
        std::cout << msg << std::endl;
        if(msg == "!NEW_SAMPLE"){
            opt->crop_save_image_sample(cropped,images,std::string(std::string(OUT_FOLDER) + std::string("/cropped_bg.jpg")));
            Entites::FILES::write_to_file(comm_file.c_str(),"$NEW_SAMPLE",' ',false,true);
        }
    }
    
    //Await changes in  mask file, with high limit
    
    delete opt;
    return 0;
}