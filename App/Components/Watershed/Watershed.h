#pragma once

#include "../Transformations/Transformations.h";
#include <opencv2/ximgproc.hpp>

class Watershed{
    public:
        /**
         * TODO
         * */
        static void background_mask(cv::Mat &src, cv::Mat &dst_mask);

        /**
         *  TODO
         * */
        static void foreground_mask(cv::Mat &src, cv::Mat &dst_mask);

    private:
        /**
         * Functions uses techniques of standard deviations and anisotropicDiffusion
         * */
        static void SD_antisotropic(cv::Mat &src, cv::Mat &dst, int SD_kernel=3, int as_iterations=3,double as_time=0.1);

        /**
         * Functions uses other technique, clacheIMG and medianblur to do first preprocessing of the image
         * */
        static void clache_medBlur(cv:: Mat &src, cv::Mat &dst, double clache_limit=4.0,int blur_kernel=7);

        /**
         *  Function performes hysteresisThresholding with const min and max threshold
         *  @param min_area - minimal size of area detected by connected components, (best set to 0)
         * */
        static void hysteresisThresholding(cv::Mat &src, cv::Mat &dst, int min_threshold, int max_threshold, int min_area);

        /**
         * Function preforms hysteresisThresholding with const min threshold, and otsu adaptive threshold for max_threshold
         * @param min_area - minimal size of area detected by connected components, (best set to 0)
         * */
        static void hysteresisThresholding(cv::Mat &src, cv::Mat &dst, int min_threshold, int min_area);

        /**
         * TODO
         * */
        static double calculate_avg_radius(cv::Mat &src, int sure_min_radius, int sure_maximum_radius);
};