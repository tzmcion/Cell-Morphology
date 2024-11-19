#pragma once

#include "../Transformations/Transformations.h"
#include <opencv2/ximgproc.hpp>

class Watershed{
    public:
        /**
         * Function creates a mask for sure background
         * @param src source_image
         * @param dst_mask output mask
         * */
        static void background_mask(cv::Mat &src, cv::Mat &dst_mask);

        /**
         *  Function returns a set of regions which can be potential foreground mask
         *  @param src source image
         *  @param dst_mask output
         *  @param bg_mask background mask
         * */
        static void foreground_regoins(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &bg_mask);

        /**
         *  Function creates a foreground mask for a image
         * */
        static void foreground_mask(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &foreground_regions, cv::Mat &sure_background);


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
         * */
        static cv::Mat createRingMatchedFilter(int size, double s1, double s2);

        /**
         * Functions uses other technique, clacheIMG and medianblur to do first preprocessing of the image
         * */
        static void clache(cv:: Mat &src, cv::Mat &dst, double clache_limit=4.0);


        //*************
        //** PRIVATE **
        //*************
        private: 

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