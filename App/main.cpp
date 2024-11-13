#include "./Components/Transformations/Transformations.h"
#include "./Components/Watershed/Watershed.h"
#include <opencv2/ximgproc.hpp>
//This File is for testing purposes

int main(int argc, char** argv) {
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    std::string path;
    for(int x = 0; x < 18; x++){
        path = "./Utils/Preprocess_all/out_2/000";
        if(x < 10){
            path += "0";
        }
        path += std::to_string(x);
        path += ".JPG";
    cv::Mat image = cv::imread(path,cv::IMREAD_GRAYSCALE);    //Import the image
    // cv::Mat total_original = cv::imread("../HIP/H08/00001.JPG");
    cv::Mat background_mask, foreground_mask;
    Watershed::background_mask(image, background_mask);
    Watershed::foreground_mask(image,foreground_mask,background_mask);
    cv::Mat img;
    image.copyTo(img);
    img.convertTo(img,CV_8U);
    cv::Mat img_color, img_wat;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);
    // Colorir a imagem para visualização
    for (int i = 0; i < foreground_mask.rows; i++) {
        for (int j = 0; j < foreground_mask.cols; j++) {
            int label = foreground_mask.at<uchar>(i, j);
            int bg_label = background_mask.at<uchar>(i, j);
            // if(bg_label != 0){
            //     img_color.at<Vec3b>(i, j) = Vec3b(255, 125, 50); // Vermelho para bordas
            // }
            if (label != 0) { // Borda
                img_color.at<Vec3b>(i, j) = Vec3b(0, 0, 255); // Vermelho para bordas
            }
        }
    }
    cv::Mat markers = cv::Mat::zeros(img.size(), CV_32S);
    background_mask = (background_mask == 0);
    markers.setTo(1,background_mask);
    markers.setTo(2,foreground_mask);
    // cv::connectedComponents(foreground_mask,markers);

    cv::Mat img_2;
    img_wat.copyTo(img_2);
    Watershed::clache_medBlur(img,img_2,2.0,5);
    cv::cvtColor(img_2, img_wat, cv::COLOR_GRAY2BGR);
    cv::watershed(img_wat,markers);

    Mat result;
    cv::cvtColor(image,result,cv::COLOR_GRAY2RGB);
    int count_green = 0;
    // img_wat.copyTo(result);
    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            int markerValue = markers.at<int>(i, j);
            if (markerValue == -1) {
                result.at<Vec3b>(i, j) = Vec3b(0, 0, 255); // Red for watershed boundaries
            }
            // else if (markerValue == 1) {
            //     result.at<Vec3b>(i, j) = Vec3b(255, 0, 0); // Blue for background
            // }
            else if (markerValue == 2) {
                count_green += 1;
                result.at<Vec3b>(i, j) = Vec3b(0, 255, 0); // Green for foreground
            }
            // else {
            //     result.at<Vec3b>(i, j) = Vec3b(markerValue * 10 % 256, markerValue * 20 % 256, markerValue * 30 % 256);
            // }
        }
    }

    Mat imageBGRA;
    cvtColor(image, imageBGRA, COLOR_BGR2RGB);

    // Blend the watershed result with the original image using 20% opacity for the result
    Mat blended;
    addWeighted(imageBGRA, 0.7, result, 0.3, 0, blended); // 80% original + 20% overlay

    // Display the result
    // imshow("Original Image", image);
    imshow("Watershed Segmentation with Opacity Overlay", blended);
    waitKey(0);
    std::cout << "RATIO: " << count_green / double(image.rows*image.cols) << '\n';
    }
    // Display results
    // imshow("Original Image", image);
    // imshow("Watershed Segmentation", result);
    // waitKey(0);




    // cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    // clahe->setClipLimit(4.0); // Adjust clip limit for different effects
    // cv::Mat claheImg;
    // clahe->apply(img, claheImg);
    // img = claheImg;
    // cv::GaussianBlur(img, img, cv::Size(7, 7), 0);
    // cv::medianBlur(img,img,7);
    // cv::Mat test;
    // Transformations::opening(test,img,'O',2);
    // cv::Mat org = test;
    // cv::imshow("RES",test);
    // cv::waitKey(0);
    // // Output image for contrast stretching
    // cv::Mat img_stretched;

    // cv::normalize(img,img_stretched,0,255,cv::NORM_MINMAX);
    // cv::Mat img_thresholded;
    // img = org;
    // double otsu_thresh_val = cv::threshold(claheImg, img_thresholded, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    // cv::Mat minimo;
    // cv::Mat minima_mask = img < (Transformations::image_brightnes(img)/10*6.5);

    // // Step 3: Detect local minima within the mask
    // // Define a kernel size for the minimum search (3x3 kernel used here)
    // cv::Mat eroded;
    // cv::dilate(img, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

    // // Compare original image to eroded image within the mask to find local minima
    // cv::Mat sure_bg;
    // cv::Mat local_minima = (img == eroded) & minima_mask;

    // cv::Mat markerLabels;
    // cv::connectedComponents(local_minima, markerLabels);
    // markerLabels = markerLabels + 1; // Para que fundo seja 1 e não 0
    // markerLabels.convertTo(markerLabels,CV_32S);

    // // Optional: Visualize local minima (mark minima in a copy of the original image)
    // cv::Mat img_minima;
    // img.copyTo(img_minima);
    // img_minima.setTo(255, local_minima); // Highlight minima in white
    // cv::imshow("Minima",img_minima);
    // cv::waitKey(0);


    // // img.convertTo(img,CV_8UC3);
    // cv::Mat img_color;
    // cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);
    // cv::watershed(img_color, markerLabels);
    // // Colorir a imagem para visualização
    // for (int i = 0; i < markerLabels.rows; i++) {
    //     for (int j = 0; j < markerLabels.cols; j++) {
    //         int label = markerLabels.at<int>(i, j);
    //         if (label == -1) { // Borda
    //             img_color.at<Vec3b>(i, j) = Vec3b(0, 0, 255); // Vermelho para bordas
    //         }
    //     }
    // }
    // cv::imshow("Edited",img_color);
    // cv::waitKey(0);
    return 0;
}