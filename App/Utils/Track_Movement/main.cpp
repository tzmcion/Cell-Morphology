/**
 * Model tracks movement of cells by trying to line 
 * 
 * 
 */
#include "../../Components/Tracking/Tracking.h"



int main(int argc, char** argv){
    std::vector<cv::Mat> patches;
    std::vector<int> res;
    double min_val = 0.0, max_val = 0.0;
    patches.push_back(cv::imread("./00001.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00009.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00017.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00006.JPG", cv::IMREAD_GRAYSCALE));
    // patches.push_back(cv::imread("./00016.JPG", cv::IMREAD_GRAYSCALE));
    Tracking::radius_by_centers(patches, "./algorithm_default.option", res, min_val, max_val);
    Tracking::cout_distribution(res, 20, min_val, max_val);
    patches[0] = cv::imread("./00009.JPG", cv::IMREAD_GRAYSCALE);
    Tracking::radius_by_centers(patches, "./algorithm_default.option", res, min_val, max_val);
    Tracking::cout_distribution(res, 20, min_val, max_val);
    patches[0] = cv::imread("./00017.JPG", cv::IMREAD_GRAYSCALE);
    Tracking::radius_by_centers(patches, "./algorithm_default.option", res, min_val, max_val);
    Tracking::cout_distribution(res, 20, min_val, max_val);
    return 0;
}