#include "Transformations.h"

double Transformations::image_brightnes(std::string path){
    cv::Mat gray = cv::imread(path, cv::IMREAD_GRAYSCALE);
    Transformations::is_image(gray,path);
    cv::Scalar mean_val = cv::mean(gray);
    return mean_val[0];
}

void Transformations::alter_brightnes(std::string path, double brightnes){
    cv::Mat o_img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    Transformations::is_image(o_img,path);
    cv::Mat img;
    cv::resize(o_img,img,cv::Size(500,500));
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

void Transformations::opening(cv::Mat &image,std::string path, char operation, int kernelSize){
    int flag_operation = 0;
    cv::Mat o_img = cv::imread(path,cv::IMREAD_GRAYSCALE);
    Transformations::is_image(o_img,path);
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
    std::cout << flag_operation << std::endl;
    cv::Mat img;
    cv::resize(o_img,img,cv::Size(1000,1000));
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(img,o_img,flag_operation,kernel);
    image = o_img;
}

void Transformations::dots_remove(cv::Mat &img, int threshold_black, int threshold_size){
    cv::Mat binary_image;
    cv::threshold(img,binary_image,110,255,cv::THRESH_BINARY);
    cv::Mat labels,stats,centroids;
    const int numLab = cv::connectedComponentsWithStats(binary_image,labels,stats,centroids,8,4);
    cv::Mat o_img = cv::Mat::zeros(binary_image.size(),0);
    for(int x = 1; x < numLab; x++){
        cv::Mat componentMask = (labels == x) * 255;
        cv::Scalar meanValue = cv::mean(img, componentMask);
        double meanBrightness = meanValue[0];
        int area = stats.at<int>(x, cv::CC_STAT_AREA);
        if(meanBrightness >= 180 && area <= 50)
        {    
            cv::bitwise_or(img,componentMask,o_img);
        }
    }
    //Let's do tomorrow gausian blur in places where the areas leave
    //Then it should do the trick
        cv::imshow("Show",o_img);
}


