#include "./Transformations.h"

double Transformations::image_brightnes(cv::Mat img){
    cv::Mat gray = img;
    Transformations::is_image(gray);
    cv::Scalar mean_val = cv::mean(gray);
    return mean_val[0];
}

void Transformations::opening(cv::Mat &image,cv::Mat src, char operation, int kernelSize){
    int flag_operation = 0;
    cv::Mat o_img = src;
    Transformations::is_image(o_img);
    switch (operation)
    {
    case 'O':
        flag_operation = cv::MORPH_OPEN;
        break;
    case 'C':
        flag_operation = cv::MORPH_CLOSE;
        break;
    case 'W':
        flag_operation = cv::MORPH_TOPHAT;
        break;
    case 'B':
        flag_operation = cv::MORPH_BLACKHAT;
        break;
    case 'G':
        flag_operation = cv::MORPH_GRADIENT;
        break;
    default:
        throw std::invalid_argument("Invalid argument in Function Transofrmations::opening()");
        break;
    }
    cv::Mat img;
    cv::resize(o_img,img,cv::Size(1000,1000));
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(img,o_img,flag_operation,kernel);
    image = o_img;
}

void Transformations::dots_remove(cv::Mat &img, int threshold_black, int threshold_size, int kern_s_dil_init, int kern_s_dil_sec, int inpaint_size, int type){
    Transformations::data_validation_dots_remove(threshold_black,threshold_size,kern_s_dil_init,kern_s_dil_sec,inpaint_size,type);
    cv::Mat binary_image;
    const double brightnes = Transformations::image_brightnes(img);
    cv::threshold(img,binary_image,(brightnes/25*threshold_black),255,cv::THRESH_BINARY_INV);
    cv::Mat labels,stats,centroids;
    const int numLab = cv::connectedComponentsWithStats(binary_image,labels,stats,centroids,4,4);
    cv::Mat o_img;  img.copyTo(o_img);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kern_s_dil_sec, kern_s_dil_sec));
    cv::Mat kernel_small = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kern_s_dil_init, kern_s_dil_init));
    std::vector<int> x_set;
    for(int x = 1; x < numLab; x++){
        cv::Mat componentMask = (labels == x);
        int area = stats.at<int>(x, cv::CC_STAT_AREA);
        if(area < threshold_size)
        {    
            x_set.push_back(x);
            cv::Mat mask;
            cv::dilate(componentMask,mask,kernel_small);
            o_img.setTo(brightnes,mask);
        }
    }
    for(size_t x = 0; x < x_set.size(); x++){
        cv::Mat componentMask = (labels == x_set[x]);
        cv::Mat mask = componentMask;
        mask.setTo(brightnes,componentMask);
        cv::dilate(componentMask,mask,kernel);
        cv::inpaint(o_img, mask, o_img, inpaint_size, type);
    }
    img = o_img;
}


void Transformations::norm_brightnes(cv::Mat &img, int max_radius, int alter_radius, int threshold){
    //Check if the image exists and is not broken
    Transformations::is_image(img);
    //First, calculate the average brightnes to which every part of the image will be altered
    const double BASE_BR = Transformations::image_brightnes(img);
    //Now, based on radius, find the circle on an image, which is the closes to the BASE_BR
    cv::Point start_point;
    double curr_closest_mean = 255; // it is mathematically imposibble to be higher than 255
    for(int x = 0; x < img.rows; x += max_radius){
        for(int y = 0; y < img.cols; y += max_radius){
            cv::Point curr_point(x,y);
            const double curr_mean = Transformations::get_mean_of_circle(img,curr_point,max_radius);
            if(abs(BASE_BR - curr_mean) < curr_closest_mean){
                curr_closest_mean = abs(BASE_BR - curr_mean);
                start_point.x = x;
                start_point.y = y;
            }
        }
    }
    //Now, we have our starting point
    //From that starting point we project "dikstra-like" alorithm to alter brightnes
    Transformations::dijkstra_mean_alter(img,start_point,BASE_BR,max_radius,alter_radius,threshold);
}

void Transformations::square_and_resize(cv::Mat &img, int d_size){
    Transformations::is_image(img);
    //Find the shorter dimension, which will be base
    int crop_size = std::min(img.rows, img.cols);

    // Define the top-left corner of the crop based on the center
    int x_offset = (img.cols - crop_size) / 2;
    int y_offset = (img.rows - crop_size) / 2;

    // Perform center crop by defining a region of interest (ROI)
    cv::Rect roi(x_offset, y_offset, crop_size, crop_size);
    cv::Mat cropped_img = img(roi);

    // Resize the cropped square image to the desired size
    cv::resize(cropped_img, img, cv::Size(d_size, d_size));
}
