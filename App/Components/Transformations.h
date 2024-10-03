#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

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


    private:

    static void is_image(cv::Mat &img, std::string path="certain_path"){
        if(img.empty()){
            const std::string msg = "Could not open the image on path: " + path;
            throw std::invalid_argument(msg);
        }
    }
};


