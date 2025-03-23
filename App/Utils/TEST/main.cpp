#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Optimalization/Optimaliation.h"
#include "../../Components/Threading/Threads.h"
#include "../../Components/Structures/AlgorithmOptions.h"

/**
    This file is for testing whatever needs to be tested in that specific moment
    it does not contain any unit/integration tests
*/

int main(int argc, char **argv){
    const std::string user_mask_path = "./data/generated_mask.bmp";
    const std::string image_path = "./data/cropped_bg.jpg";
    const std::string OPTIONS_PATH = "./data/algorithm_default.option";

    
    Optimalization opt = Optimalization();
    opt.read_user_mask(user_mask_path);
    //Create new mask
    AlgorithmOptions options(OPTIONS_PATH.c_str());
    cv::Mat image;
    image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    opt.single_option_optimization(image,options,"WATERSHED",4,true);
    opt.single_option_optimization(image,options,"WATERSHED",2,true);
    opt.single_option_optimization(image,options,"WATERSHED",1,true);
    opt.single_option_optimization(image,options,"WATERSHED",0,true);
    //
    return 0;
}