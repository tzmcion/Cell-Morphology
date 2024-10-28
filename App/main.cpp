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
    cv::Mat img = cv::imread("./Utils/Norm_noise/out_seco/T2.JPG",cv::IMREAD_GRAYSCALE);
    cv::Mat org = img;
    // cv::Mat img = cv::imread("./Test/T2.JPG",cv::IMREAD_GRAYSCALE);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4.0); // Adjust clip limit for different effects
    cv::Mat claheImg;
    clahe->apply(img, claheImg);
    img = claheImg;

    int kernelSize = 10;  // Adjust based on the desired neighborhood size
    cv::Mat d;
    cv::GaussianBlur(img, img, cv::Size(7, 7), 0);
    cv::medianBlur(img,img,7);
    cv::Mat test;
    Transformations::opening(test,img,'O',2);
    // cv::Ptr<cv::CLAHE> clahee = cv::createCLAHE();
    // clahee->setClipLimit(2.0); // Adjust clip limit for different effects
    // cv::Mat claheImgg;
    // clahee->apply(test, claheImgg);
    org = test;
    cv::imshow("RES",test);
    cv::waitKey(0);
    Transformations::opening(test,img,'C',3);
    cv::waitKey(0);
    Transformations::opening(test,img,'W');
    cv::imshow("RES",test);
    cv::waitKey(0);
    Transformations::opening(test,img,'B');
    cv::imshow("RES",test);
    cv::waitKey(0);
    Transformations::opening(test,img,'G');
    cv::imshow("RES",test);
    cv::waitKey(0);
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

    cv::normalize(img,img_stretched,0,255,cv::NORM_MINMAX);
    cv::Mat img_thresholded;
    img = org;
    double otsu_thresh_val = cv::threshold(claheImg, img_thresholded, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::Mat minimo;
    cv::Mat minima_mask = img < (Transformations::image_brightnes(img)/10*6.5);

    // Step 3: Detect local minima within the mask
    // Define a kernel size for the minimum search (3x3 kernel used here)
    cv::Mat eroded;
    cv::dilate(img, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

    // Compare original image to eroded image within the mask to find local minima
    cv::Mat local_minima = (img == eroded) & minima_mask;

    // Optional: Visualize local minima (mark minima in a copy of the original image)
    cv::Mat img_minima;
    img.copyTo(img_minima);
    img_minima.setTo(255, local_minima); // Highlight minima in white

    cv::Mat img_minima_maxima;
    cv::cvtColor(img, img_minima_maxima, cv::COLOR_GRAY2BGR); // Convert to color for visualization
    img_minima_maxima.setTo(cv::Scalar(0, 0, 255), local_minima); // Mark minima in red
    // img_minima_maxima.setTo(cv::Scalar(0, 255, 0), local_maxima); // Mark maxima in green


    cv::medianBlur(claheImg,claheImg,3);
    cv::imshow("original",img_thresholded);
    cv::imshow("Edited",img_minima);
    cv::imshow("Maxima",img_minima_maxima);

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