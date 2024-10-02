#include "Transformations.h"

double Transformations::image_brightnes(std::string path){
    cv::Mat gray = cv::imread(path, cv::IMREAD_GRAYSCALE);
    Transformations::is_image(gray,path);
    cv::Scalar mean_val = cv::mean(gray);
    return mean_val[0];
}

void Transformations::alter_brightnes(std::string path, double brightnes){
    cv::Mat o_img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::Mat img;
    cv::resize(o_img,img,cv::Size(500,500));
    Transformations::is_image(img,path);
    cv::Mat new_img = cv::Mat::zeros(img.size(),img.type());
    const double br = -1*(cv::mean(img)[0] - brightnes);
    const double alpha = 1;
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            new_img.at<uchar>(x,y) = cv::saturate_cast<uchar>(alpha*img.at<uchar>(x,y) + br);
        }
    }
    cv::imshow("Original:", img);
    cv::imshow("Altered",new_img);
    cv::waitKey(0);
}



