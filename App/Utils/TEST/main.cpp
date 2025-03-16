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
    cv::Mat background_mask, foreground_mask, foreground_regions, watershed_mask, result;
    Watershed::background_mask(image,background_mask,options.get_int_var(0,"BACKGROUND_MASK"),options.get_int_var(1,"BACKGROUND_MASK"),options.get_db_var(2,"BACKGROUND_MASK"),options.get_int_var(3,"BACKGROUND_MASK"),options.get_int_var(4,"BACKGROUND_MASK"),options.get_int_var(5,"BACKGROUND_MASK"),options.get_db_var(6,"BACKGROUND_MASK"),options.get_int_var(7,"BACKGROUND_MASK"));
    // //This will be even longer
    Watershed::foreground_regions(image,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_REGIONS"),options.get_int_var(1,"FOREGROUND_REGIONS"),options.get_db_var(2,"FOREGROUND_REGIONS"),options.get_int_var(3,"FOREGROUND_REGIONS"),options.get_db_var(4,"FOREGROUND_REGIONS"),options.get_int_var(5,"FOREGROUND_REGIONS"));
    // //And thiss will be the longest
    Watershed::foreground_mask(image,foreground_mask,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_MASK"), options.get_int_var(1,"FOREGROUND_MASK"), options.get_int_var(2,"FOREGROUND_MASK"), options.get_int_var(3,"FOREGROUND_MASK"), options.get_db_var(4,"FOREGROUND_MASK"), options.get_db_var(5,"FOREGROUND_MASK"),options.get_db_var(6,"FOREGROUND_MASK"), options.get_db_var(7,"FOREGROUND_MASK"), options.get_int_var(8,"FOREGROUND_MASK"), options.get_int_var(9,"FOREGROUND_MASK"), options.get_int_var(10,"FOREGROUND_MASK"), options.get_db_var(11,"FOREGROUND_MASK"), options.get_int_var(12,"FOREGROUND_MASK"));
    //And apply the watershed with masks and options
    Watershed::watershed_with_masks(image,watershed_mask,background_mask,foreground_mask,options.get_int_var(0,"WATERSHED"),options.get_int_var(1,"WATERSHED"),options.get_int_var(2,"WATERSHED"),options.get_db_var(3,"WATERSHED"),options.get_int_var(4,"WATERSHED"));
    //new sample should be in variable watershed_mask
    Watershed::draw_watershed_lines(image,result,watershed_mask);
    // std::cout << "Oversection_over_union: ";
    std::cout << opt.calculate_intersection_over_union(watershed_mask) << std::endl;
    // cv::imshow("SHOW",result);
    // cv::waitKey(0);
    return 0;
}