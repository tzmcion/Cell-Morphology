#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Optimalization/Optimaliation.h"
#include "../../Components/Threading/Threads.h"
#include "../../Components/Structures/AlgorithmOptions.h"
#include <algorithm>
#include <random> // For std::mt19937 and std::random_device
/**
    This file is for testing whatever needs to be tested in that specific moment
    it does not contain any unit/integration tests
*/

int main(int argc, char **argv){
    const std::string user_mask_path = "./data/generated_mask.bmp";
    const std::string image_path = "./data/cropped_bg.jpg";
    const std::string OPTIONS_PATH = "./data/algorithm_default.option";
    std::random_device rd;  // Non-deterministic random seed
    std::mt19937 g(rd());   // Mersenne Twister PRNG
    
    Optimalization opt = Optimalization();
    opt.read_user_mask(user_mask_path);
    //Create new mask
    AlgorithmOptions options(OPTIONS_PATH.c_str());
    AlgorithmOptions original(OPTIONS_PATH.c_str());
    cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    cv::Mat super_result;

    struct opt_option{
        opt_option(std::string name, int index, bool is_iter, int max_iter, int min_iter, double resolution){
            this->name = name;
            this->index = index;
            this->is_iter = is_iter;
            this->max_iter = max_iter;
            this->min_iter = min_iter;
            this->resolution = resolution;
        }
        std::string name;
        int index;
        bool is_iter;
        int max_iter;
        int min_iter;
        double resolution;
    };
    double best_ever_val = 0.0, total_best = 0.0;
    std::vector<opt_option> options_to_optimize;
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",0,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",1,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",0,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",0,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",2,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",3,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",3,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",2,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",8,false,50,10,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",9,false,10,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",5,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",4,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",12,false,5,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",1,true,20,1,0.1));
    options_to_optimize.push_back(opt_option("WATERSHED",1,true,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",1,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("BACKGROUND_MASK",2,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("BACKGROUND_MASK",6,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",2,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",4,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",4,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",5,false,30,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",6,false,30,1,0.1));
    // options_to_optimize.push_back(opt_option("FOREGROUND_MASK",7,false,30,1,0.1));
    // options_to_optimize.push_back(opt_option("WATERSHED",4,false,30,1,0.1));

    for(size_t y = 0; y < 1000; y++){
        AlgorithmOptions opt_temp(OPTIONS_PATH.c_str());
        for(size_t x = 0; x < options_to_optimize.size(); x++){
            best_ever_val = opt.single_option_optimization(image,opt_temp,options_to_optimize[x].name,options_to_optimize[x].index,options_to_optimize[x].is_iter,options_to_optimize[x].max_iter,options_to_optimize[x].min_iter,options_to_optimize[x].resolution);
        }
        if(best_ever_val > total_best){
            total_best = best_ever_val;
            opt.simulate_watershed(image, super_result, opt_temp);
        }
        std::shuffle(options_to_optimize.begin(), options_to_optimize.end(),g);
    }
    std::cout << "BEST EVER VAL:" << total_best << std::endl;
    cv::Mat result, org_result, mask_result, mask_org;
    opt.simulate_watershed(image, mask_org, original);
    Watershed::draw_watershed_lines(image, result, super_result);
    Watershed::draw_watershed_lines(image, org_result, mask_org);
    cv::imshow("Result optimization", result);
    cv::imshow("Beofre Opt", org_result);
    cv::waitKey(0);
    //
    return 0;
}