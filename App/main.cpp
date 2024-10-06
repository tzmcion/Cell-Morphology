#include "./Components/Transformations.h"

//Transformation of set of images given in a string input, and smoothen of the brightness

int main(int argc, char** argv) {
    // if(argc < 2){
    //     throw std::invalid_argument("SubComponent requires list of images to calculate mean");
    // }
    // const std::string comp_list = argv[1];
    // std::cout << comp_list;
    // const double brightnes = Transformations::image_brightnes("./Test/T1.JPG");
    // std::cout << "Average brightnes of the image is: " << brightnes << std::endl;
    //Transformations::alter_brightnes("./Test/T1.JPG",brightnes);
    cv::Mat img = cv::imread("./Test/T2.JPG",cv::IMREAD_GRAYSCALE);
    cv::Mat d_img;
    cv::resize(img,d_img,cv::Size(1000,1000));
    //cv::equalizeHist(d_img,img);
    cv::imshow("original",d_img);
    Transformations::norm_brightnes(d_img,50);
    cv::imshow("altered brightnes",d_img);
    //Transformations::dots_remove(d_img);
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