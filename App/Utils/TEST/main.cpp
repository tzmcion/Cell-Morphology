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
    AlgorithmOptions original(OPTIONS_PATH.c_str());
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",0,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",1,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_REGIONS",0,true,20);
    opt.single_option_optimization(image,options,"WATERSHED",0,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",2,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",10,true,500,200);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",3,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_REGIONS",3,true,20);
    opt.single_option_optimization(image,options,"WATERSHED",2,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",8,false,50,10);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",9,false,10);
    opt.single_option_optimization(image,options,"FOREGROUND_REGIONS",5,true,20);
    opt.single_option_optimization(image,options,"WATERSHED",4,true,20);
    opt.single_option_optimization(image,options,"FOREGROUND_MASK",12,false,5);
    opt.single_option_optimization(image,options,"FOREGROUND_REGIONS",1,true,20);
    opt.single_option_optimization(image,options,"WATERSHED",1,true,20);
    cv::Mat result, org_result, mask_result, mask_org;
    opt.simulate_watershed(image, mask_result, options);
    opt.simulate_watershed(image, mask_org, original);
    Watershed::draw_watershed_lines(image, result, mask_result);
    Watershed::draw_watershed_lines(image, org_result, mask_org);
    cv::imshow("Result optimization", result);
    cv::imshow("Beofre Opt", org_result);
    cv::waitKey(0);
    //
    return 0;
}