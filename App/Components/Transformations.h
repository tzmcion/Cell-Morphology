#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>

class Transformations{
    public:
    /*Function returns average brightnes of the image*/
    static double image_brightnes(cv::Mat img);

    /*Function alters the image brightnes within the file (opens and saves it)*/
    static void alter_brightnes(std::string path, double brightnes);

    /**
     *  Function performes Morphological opening, the operation can be:
     *  operation='O' -> Morphological Opening
     *  operation='C' -> Morphological Close
     *  operation='W' -> White Top-Hat
     *  operation='B' -> Black Top-Hat
     *  operation='G' -> Gradient
     * */
    static void opening(cv::Mat &image,cv::Mat src, char operation, int kernelSize=5);

    /**
     *  Function removes dots using connected components, can be removed by size and
     *  @param threshold_black states how black mean of object can be
     *  @param threshold_size states min size of object which is accepted
     * */
    static void dots_remove(cv::Mat &img, int threshold_black=100, int threshold_size=50);

    /**
     *  Function normalizes brightnes of the image
     *  @param img cv.Mat imag3
     *  @param max_radius Maximum radius of the mean brightness of the area
     * */
    static void norm_brightnes(cv::Mat &img, int max_radius=50);


    private:

    static void is_image(cv::Mat &img, std::string path="certain_path"){
        if(img.empty()){
            const std::string msg = "Could not open the image on path: " + path;
            throw std::invalid_argument(msg);
        }
    }

    /**
     *  Function return mean of brightnes of circle on the image
     * */
    static double get_mean_of_circle(cv::Mat &img, cv::Point center, int radius){
        //literally the same code as in alter_ without altering
                const int rows=img.rows, cols=img.cols;
        const int x_max = std::min(rows,center.x + radius);
        const int x_min = std::max(0,center.x - radius);
        const int y_max = std::min(cols,center.y + radius);
        const int y_min = std::max(0,center.y - radius);
        //Now, in a lop calculate distance
        //If distance from the center is less than radius, then the pixel is a part of the wheel
        //And will be altered
        int count = 0;
        double sum = 0;
        for(int x = x_min; x < x_max; x++){
            for(int y = y_min; y < y_max; y++){
                const int distance = std::floor(std::sqrt(std::pow(x - center.x,2) + std::pow(y - center.y,2)));
                if(distance <= radius){
                    sum += img.at<cv::uint8_t>(x,y);
                    count += 1;
                }
            }
        }
        return sum/count;
    }

    /**
     *  Function alters brightnes on a circle
     * */
    static void alter_mean_of_circle(cv::Mat &img, cv::Point center, int goal_brightness, int radius=50){
        //First, calculate boundries
        const int rows=img.rows, cols=img.cols;
        const int x_max = std::min(rows,center.x + radius);
        const int x_min = std::max(0,center.x - radius);
        const int y_max = std::min(cols,center.y + radius);
        const int y_min = std::max(0,center.y - radius);
        //Now, in a lop calculate distance
        //If distance from the center is less than radius, then the pixel is a part of the wheel
        //And will be altered
        int count = 0;
        double sum = 0;
        std::vector<cv::Point> points;
        for(int x = x_min; x < x_max; x++){
            for(int y = y_min; y < y_max; y++){
                const int distance = std::floor(std::sqrt(std::pow(x - center.x,2) + std::pow(y - center.y,2)));
                if(distance <= radius){
                    sum += img.at<cv::uint8_t>(x,y);
                    count += 1;
                    cv::Point ap_point(x,y);
                    points.push_back(ap_point);
                }
            }
        }
        const double ajd_mean = goal_brightness - sum/count;
        //Now, the loop has to be done again to alter the points depending on brightnes
        for(int i = 0; i < points.size(); i++){
            const int x = points[i].x;
            const int y = points[i].y;
            img.at<cv::uint8_t>(x,y) = cv::saturate_cast<uchar>(img.at<uchar>(x,y) + ajd_mean);
        }
        //Should work :)
    }

    static void dijkstra_mean_alter(cv::Mat &img,cv::Point center, int brightnes, int radius){
        //End recurency
        if(center.x < 0 || center.x > img.rows || center.y < 0 || center.y > img.cols)
        //Perform operation
        alter_mean_of_circle(img,center,brightnes,radius);
        //Call recurency
        //It will work badly, needs to be done with a Heap or Pile
        // dijkstra_mean_alter(img,cv::Point(center.x + radius, center.y + radius),brightnes,radius);
        // dijkstra_mean_alter(img,cv::Point(center.x + radius, center.y - radius),brightnes,radius);
        // dijkstra_mean_alter(img,cv::Point(center.x - radius, center.y + radius),brightnes,radius);
        // dijkstra_mean_alter(img,cv::Point(center.x - radius, center.y - radius),brightnes,radius);
    }
};


