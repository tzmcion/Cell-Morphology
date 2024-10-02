#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <stdexcept>

class Transformations{
    public:
    /*Function returns average brightnes of the image*/
    static double image_brightnes(std::string path);

    /*Function alters the image brightnes within the file (opens and saves it)*/
    static void alter_brightnes(std::string path, double brightnes);


    private:

    static void is_image(cv::Mat &img, std::string path="certain_path"){
        if(img.empty()){
            const std::string msg = "Could not open the image on path: " + path;
            throw std::invalid_argument(msg);
        }
    }
};


