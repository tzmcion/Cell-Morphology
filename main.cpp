#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("image.jpg");
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    
    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}