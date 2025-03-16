#pragma once

#include "../Transformations/Transformations.h"
#include <opencv2/ximgproc.hpp>

class Watershed{
    public:
        /**
         * Function creates a mask for sure background with default argument values
         * It uses standard deviation to emphesize potential cell walls (as they are brighter)
         * the, the walls are computed as strong points in hysterisis thresholding
         * With that, the background mask is created
         * @param src source_image
         * @param dst_mask output mask
         * */
        static void background_mask(cv::Mat &src, cv::Mat &dst_mask);

        /**
         * Function creates a mask for sure background with provided options
         * It uses standard deviation to emphesize potential cell walls (as they are brighter)
         * the, the walls are computed as strong points in hysterisis thresholding
         * With that, the background mask is created
         * @param src   source image
         * @param dst_mask  destination image/mask (output)
         * @param SD_kernel Size of kernel for standard deviation, bigger->more emphesized, but more noise remains
         * @param as_iterations iterations of anisotropic filter
         * @param as_time   time for anisotropic filter
         * @param hs_min    hysterisisThreshold min threshold (weak points)
         * @param hs_max    hysterisisThreshold max threshold (strong points)
         * @param hs_min_area   hysterisisThreshold min area (minimum area of strong points)
         * @param max_hole_size Inpianting holes left by HS, max size of a hole is defined here
         * @param erosion_size  Kernel size of erosion, 1=3, so 3 equals one, 2 equals 5, 3 equals 7 etc... 
         * */
        static void background_mask(cv::Mat &src, cv::Mat &dst_mask, int SD_kernel, int as_iterations, double as_time, int hs_min, int hs_max, int hs_min_area, double max_hole_size, int erosion_size);

        /**
         *  Function returns a set of regions which can be potential foreground mask with default argument values
         *  Function performes a serie of blurs, morphs opening, and smoothing of the image to make it as fluent as possible
         *  Then, using threshold value slightly smaller than mean threshold of image, it localizes potential local minimas
         *  Those regions can be used for: 1. Foreground mask creation, 2. Calculation of total surface occupied by cells
         *  @param src source image
         *  @param dst_mask output
         *  @param bg_mask background mask
         * */
        static void foreground_regions(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &bg_mask);

        /**
         * Function creates a prototype maskk for foreground_mask -- mutlipel regions on which the mask can be applied
         *  Function performes a serie of blurs, morphs opening, and smoothing of the image to make it as fluent as possible
         *  Then, using threshold value slightly smaller than mean threshold of image, it localizes potential local minimas
         *  Those regions can be used for: 1. Foreground mask creation, 2. Calculation of total surface occupied by cells
         * @param src   source image
         * @param dst_mask  destination mask (output)
         * @param bg_mask   background mask calculated prior
         * @param blur_kernel   kernel size of initial blur
         * @param morph_opening_kernel  kernel size of morphological opening
         * @param clache_force  force of clache
         * @param second_blur   kernel size of second blur
         * @param minima_threshold  threshold multiplier for minima (mean_brightness*minima_threshold)
         * @param minima_kernel kernel size for minima dilation
         * */
        static void foreground_regions(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &bg_mask, int blur_kernel, int morph_opening_kernel, double clache_force, int second_blur_kernel, double minima_threshold, int minima_kernel);

        /**
         * Function creates a foreground mask for a image with default argument values. 
         *  Function uses Halo filter with gaussian kernel, to localize potentail cell centers. Then, after localizing
         * Those points, they are assigned to their respective foreground region, and are divided by cell radius 
         * @param src   source image
         * @param dst_mask  destination mask (output)
         * @param foreground_regions    foreground regions calculated prior
         * @param sure_background   sure background calculated before
         * */
        static void foreground_mask(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &foreground_regions, cv::Mat &sure_background);

        /**
         * Function creates a foreground mask for image
         * Function uses Halo filter with gaussian kernel, to localize potentail cell centers. Then, after localizing
         * Those points, they are assigned to their respective foreground region, and are divided by cell radius 
         * @param src   source image
         * @param dst_mask  destination mask (output)
         * @param foreground_regions    foreground regions calculated prior
         * @param sure_background   sure background calculated prior
         * @param blur_kernel   size of kernel blur
         * @param dil_er_size   size of morphological opening
         * @param SD_kernel_size    size of kernel in standard deviation
         * @param iterations    !OBSECOLETE!
         * @param sigma_min     minimum signa on gaussian kernel
         * @param sigma_max     maximum sigma on gaussian kernel
         * @param sigma_iterator    sigma iterator, so if sigma_min=2.0, sigma_max=3.0, sigma_iterator = 0.2, then 5 iterations will occure, respecting different possible cell radiuses
         * @param sigma_multiplier  second sigma multiplied by this value: sigma_2 = sigma_1 * sqrt(sigma_multiplier)
         * @param cell_radius   potential cell radius
         * @param filter_kernel_size_multiplier size of kernel for gaussian kernel: kernel_size_gaus = cell_size * filter_kernel_size_multiplier
         * @param gftt_corners  max corners for good features to track
         * @param gftt_quality  quality for good features to track
         * @param cell_radius_multiplier    multiplier of cell radius for finding closes foreground region
         * */
        static void foreground_mask(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &foreground_regions, cv::Mat &sure_background, int blur_kernel, int dil_er_size, int SD_kernel_size, int iterations, double sigma_min, double sigma_max, double sigma_iterator, double sigma_multiplier, int cell_radius, int filter_kernel_size_multiplier, int gftt_corners, double gftt_quality, int cell_radius_multiplier);


        /**
         * Function performes watershed with given background mask and foreground mask
         * @return watershed markers
         * @param src
         * @param dst
         * @param background
         * @param foreground
         * @param opening_force
         * @param blur_force
         * @param mean_median_force
         * @param clache_force
         * @param med_blur_second
         */

         static void watershed_with_masks(cv::Mat &src, cv::Mat &dst, cv::Mat &background, cv::Mat &foreground, int opening_force, int blur_force, int mean_median_force, double clache_force, int med_blur_second);

                 /**
         * Functions uses other technique, clacheIMG and medianblur to do first preprocessing of the image
         * @param src source image
         * @param dst destination image
         * @param clache_limit a force of clache filter
         * */
        static void clache(cv:: Mat &src, cv::Mat &dst, double clache_limit=4.0);

        /**
         * Function draws the line on the image, making it visually distinguishable
         * @param src
         * @param dst
         * @param watershed_mask
         */
        static void draw_watershed_lines(cv::Mat &src, cv::Mat &dst, cv::Mat &watershed_mask);

    //*************
    //** PRIVATE **
    //*************
    // private: 

        /**
         * Function peformes standatd deviation on an image, with given kernel
         * Standard deviation follows this equation
         * SD = sqrt(var)
         * VAR(X) = E(X*2) - [E(X)]^2
         * @param src source image
         * @param dst output
         * @param kernel define kernel size. Kernel defines how big part of the image is used to calculate standard deviation on a point
         * */
        static void Standard_deviation(cv::Mat &src, cv::Mat &dst, int kernel);

        /**
         * Function preforms hysteresisThresholding, which is threshold consisting of 2 points, strong points and weak points.
         * Function creates a mask of strong points and adds weak points to this mask, in places where weak points are in touching distance to the mask
         * The mask is returned as dst.
         * @param src source image
         * @param dst output
         * @param min_threshold threshold for weak points
         * @param max_threshold threshold for strong points
         * @param min_area - minimal size of area detected by connected components, (best set to 0)
         * */
        static void hysteresisThresholding(cv::Mat &src, cv::Mat &dst, int min_threshold, int max_threshold, int min_area);

        /**
         * Function preforms hysteresisThresholding, which is threshold consisting of 2 points, strong points and weak points.
         * Function creates a mask of strong points and adds weak points to this mask, in places where weak points are in touching distance to the mask
         * The mask is returned as dst, this type of hT uses otsu threshold for strong points
         * @param src source image
         * @param dst output
         * @param min_threshold threshold for weak points
         * @param min_area - minimal size of area detected by connected components, (best set to 0)
         * */
        static void hysteresisThresholding(cv::Mat &src, cv::Mat &dst, int min_threshold, int min_area);

        /**
         * Function calculates the local extremes based on mean brightness of the image
         * Local minima are regions which are declared as lower than mean brightness on the image by certain multiplicator
         * Please note that this function creates unconnected local minima areas
         * @param src source image
         * @param dst_mask output material, as mask with values <int>[0,1]
         * @param mean_multiplicator threshold base for detecting regions counted as minima
         * @param kernel kernel size for dilation of the regions.
         * */
        static void calc_local_minima(cv::Mat &src, cv::Mat &dst_mask, double mean_multiplicator=0.9, int kernel=3);
        
        /**
         * !!!TODO!!!
         * !!!NOT READY!!!
         * Function calculates predicted radius of cells on the image, using background to detect cells which are alone on the image (not in cluster)
         * If no cell like that is found, function will return (sure_min_radius+sure_maximum_radius)/2
         * @param src background_mask of the image
         * @param sure_min_radius known value of cell radius which every cell suffice (this value or more)
         * @param sure_maximum_radius known value of cell radius which none of the cells exceed
         * */
        static double avg_radius_from_mask(cv::Mat &src, int sure_min_radius = 10, int sure_maximum_radius=100);

        /**
         *  Function creates RingFilter by givven two values of SD. it favors circullar shapes by highlighting the center
         *  of the cell, creating "Halo" Effect.Function creates two gaussian 2D distributions, and creates kernel which follows this equation:
         *  -- DECLARE: G1(size,sigma_1), G2(size,sigma_2)  
         *  -- DECLARE: X as a set of pair (x,y) with cardinality equal to size*size
         *  -- DECLARE: F(X) --> Y, Y is a kernel
         *  --
         *  -- CALCULATE:F(X)={ G1<x,y>  when    G1<x,y> > G2<x,y> ; 0 otherwise }
         *  --
         *  -- RETURN:  Y
         * @param size size of kernel which will be created
         * @param s1 sigma value of first gaussian distribution
         * @param s2 sigma value of second gaussian distribution
         * */
        static cv::Mat createRingMatchedFilter(int size, double s1, double s2, bool reverse=false);

        /**
         *  Function creates a gaussian kernel, with set SD
         *  RETURN: Gaussian kernel with given size and SD
         * @param size size of kernel
         * @param sigma SD of gaussian distribution
         * */
        static cv::Mat GaussianKernel(int size, double sigma);

        /**
         * Function paints the black holes which can appear in regions of sure background.
         * @param src source image
         * @param dst output material
         * @param max_area maximum area (as sum of pixels) which can be painted
         * */
        static void white_inpaint_holes(cv::Mat &src, cv::Mat &dst, int max_area = 50);

        /**
         * Functions performes anistoropic diffusion on a grayscale image
         * @param src source image
         * @param dst destination image
         * @param as_iterations iterations of anisotropic, more iterations result in more blur of inside
         * @param as_time value of time can be described as force of anisotropic
         * */
        static void Anisotropic(cv::Mat &src, cv::Mat &dst, int as_iterations=3,double as_time=0.1);

        /**
         *  Function checks if any object on the surface map is in radius of a point
         *  Object is a field which value is different from zero
         *  for(auto object in surface)
         *  --IF(distance(p,object) < radius) return true
         *  @param p point from which distance is measured
         *  @param surface surface on which object can appear
         *  @param radius radius in which object needs to be detected for function to return true
         * */
        static bool is_object_in_radius(cv::Point p, cv::Mat &surface, int radius);
};