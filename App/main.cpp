#include "./Components/Transformations/Transformations.h"

//This File is for testing purposes

int main(int argc, char** argv) {
    // if(argc < 2){
    //     throw std::invalid_argument("SubComponent requires list of images to calculate mean");
    // }
    // const std::string comp_list = argv[1];
    // std::cout << comp_list;
    // const double brightnes = Transformations::image_brightnes("./Test/T1.JPG");
    // std::cout << "Average brightnes of the image is: " << brightnes << std::endl;
    //Transformations::alter_brightnes("./Test/T1.JPG",brightnes);
    cv::Mat img = cv::imread("./Utils/Norm_noise/out_final/T2.JPG",cv::IMREAD_GRAYSCALE);
    // cv::Mat img = cv::imread("./Test/T2.JPG",cv::IMREAD_GRAYSCALE);

    int kernelSize = 10;  // Adjust based on the desired neighborhood size
    cv::Mat d;
    cv::medianBlur(img,img,5);
    // cv::bilateralFilter(img,d,9,150,150);

    // // Calculate the local min and max using erode and dilate
    // cv::Mat localMin, localMax;
    // cv::erode(img, localMin, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize)));
    // cv::dilate(img, localMax, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize)));

    // // Initialize the output image for local contrast stretching
    // cv::Mat img_stretched = cv::Mat::zeros(img.size(), CV_8UC1);

    // // Apply local contrast stretching
    // for (int y = 0; y < img.rows; y++) {
    //     for (int x = 0; x < img.cols; x++) {
    //         int minVal = localMin.at<uchar>(y, x);
    //         int maxVal = localMax.at<uchar>(y, x);

    //         // Avoid division by zero
    //         if (maxVal > minVal) {
    //             img_stretched.at<uchar>(y, x) = cv::saturate_cast<uchar>(255.0 * (img.at<uchar>(y, x) - minVal) / (maxVal - minVal));
    //         } else {
    //             img_stretched.at<uchar>(y, x) = img.at<uchar>(y, x);
    //         }
    //     }
    // }

    // Output image for contrast stretching
    cv::Mat img_stretched;

    // Perform global contrast stretching
    double minVal, maxVal;
    cv::minMaxLoc(img, &minVal, &maxVal); // Find minimum and maximum pixel values

    // Stretching the pixel values to the range [0, 255]
    img.convertTo(img_stretched, CV_8UC1, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));



    cv::imshow("original",img);
    cv::waitKey(0);
    // const char operations[5] = {'O','C','W','B','G'};
    // for(int x = 0; x < 5; x++){
    //     cv::Mat c_img; 
    //     Transformations::opening(c_img,"./Test/T2.JPG",operations[x]);
    //     cv::imshow("changed",c_img);
    //     cv::waitKey(0);
    // }

    return 0;
}