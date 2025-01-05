#pragma once

#include "./Basic_Transformations.h"
#include "../Structures/Structures.h"

class Transformations: public BasicTransofrmations{
    //!! PRIVATE FIRST HERE!!//
    private:

    /**
     *  Function validates values for function dots_remove and quits the program if values are out of bonds
     * */
    static void data_validation_dots_remove(int threshold_black, int threshold_size, int ker1, int ker2, int inp, int type);

    /**
     *  Function return mean of brightnes of circle on the image
     * */
    static double get_mean_of_circle(cv::Mat &img, cv::Point center, int radius = 50);

    /**
     *  Function alters brightnes on a circle with calculation od it's influence.
     *  The force of altere is defined by it's distance from the center. If it's equal to radius, then it's 0*calc_alter
     *  If it's hald of radius, then 0.5*calc_alter etc.
     * */
    static void alter_mean_of_circle(cv::Mat &img, cv::Point center, int goal_brightness, int radius=50, int threshold_val = 20);

    /**
     * This is a dijkstra point, which stores the pointer to the already visited point's, and the position of it's center
     * **/
    struct dijkstra_point{
        dijkstra_point(cv::Point c, std::vector<cv::Point> *v):center(c), visited(v){};
        cv::Point center;
        std::vector<cv::Point> *visited;
    };


    /**
     * 4-way application of dijkstra_mean_alter algorithm described in article
     * */
    static void dijkstra_mean_alter(cv::Mat &img_o,cv::Point start_point, int brightnes,int radius = 50, int alter_radius = 100, int threshold=20);
//
//    
//***********************
//********PUBLIC*********
//***********************
//
//
    public:
    /*Function returns average brightnes of the image*/
    static double image_brightnes(cv::Mat &img);

    /*Function returns average brightness on the image excluding dark regions */
    static double image_brightnes(cv::Mat &img, double thr_to_exclude);

    /*Function alters the provided image, it ADDS the provided brightness to every pixel
        So, the brightness value can be either a positive or a negative number
    */
    static void alter_brightnes(cv::Mat &img, double brightnes);

    /**
     *  Function performes Morphological opening, the operation on 'operation' argument can be:
     *  operation='O' -> Morphological Opening
     *  operation='C' -> Morphological Close
     *  operation='W' -> White Top-Hat
     *  operation='B' -> Black Top-Hat
     *  operation='G' -> Gradient
     * */
    static void opening(cv::Mat &image,cv::Mat src, char operation, int kernelSize=5);

    /**
     *  Function removes dots using connected components, can be removed by size and acceptance threshold
     *  Function paints over the detected components, painting them to the color of mean brightness of the image.
     *  @param threshold_black Threshold value for detecting the object as removable (ratio -- mean/25 * threshold_black)
     *  @param threshold_size maximum size of connected components, bigger ones than provided are excluded from removal
     *  @param kern_s_dil_init size of initial dilation, before the painting of gray over the detected object 
     *  @param kern_s_dil_sec size of second dilation, after the painting of gray over the detected object
     *  @param inpaint_size size of kernel of inpaint, inpaint merges the objects with background
     *  @param type type of inpaint algorithm, 1 or 0
     *  @param display_changes Can be set to true if you wish to get a openCV window which shows step by step how the data changes 
     * */
    static void dots_remove(cv::Mat &img, int threshold_black=15, int threshold_size=50, int kern_s_dil_init = 3, int kern_s_dil_sec = 5, int inpaint_size = 5, int type = 1, bool display_changes = false);

    /**
     *  Function normalizes brightnes of the image 
     *  @param img cv.Mat imag3
     *  @param max_radius Maximum radius of the mean brightness of the area
     * */
    static void norm_brightnes(cv::Mat &img, int max_radius=50, int alter_radius=200, int threshold=30);

    /**
     * Function normalizes brightness on the image and thrives with it to the goal brightness
     * @param img cv.Mat image
     * @param goal_brightness goal brightness to which the image will thrive
     * */
    static void norm_brightnes(cv::Mat &img, double goal_brightness, int max_radius=50, int alter_radius=200, int threshold=30);

    /**
     *  Function crops the image to be a square, calculating from the middle
     *  @param img CV.Mat image to alter
     *  @param d_size desired size of image (width)
     * */
    static void square_and_resize(cv::Mat &img, int d_size = 1000);
};


