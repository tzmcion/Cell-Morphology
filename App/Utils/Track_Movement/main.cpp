/**
 * Model tracks movement of cells by trying to line 
 * 
 * 
 */
#include "../../Components/Tracking/Tracking.h"



int main(int argc, char** argv){
    std::vector<cv::Mat> patches;
    std::vector<int> res;
    double min_val = 0.0, max_val = 0.0, avg = 0.0;
    patches.push_back(cv::imread("./00001..jpg", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00009.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00017.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00006.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00016.JPG", cv::IMREAD_GRAYSCALE));
    cv::Mat imgg = patches[0];
    Tracking::radius_by_centers(imgg, "./algorithm_default.option", res, min_val, max_val, avg);
    Tracking::cout_distribution(res, 20, min_val, max_val);
    return 0;
}