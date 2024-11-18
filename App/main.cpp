#include "./Components/Transformations/Transformations.h"
#include "./Components/Watershed/Watershed.h"
#include <opencv2/ximgproc.hpp>
//This File is for testing purposes

int main(int argc, char** argv) {
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
        std::string path;
        cv::Mat original = cv::imread("./Utils/Preprocess_all/out/00017 copy.JPG");
    for(int x = 6; x < 15; x++){
        path = "./Utils/Preprocess_all/out/00017 copy.JPG";
        // if(x < 10){
        //     path += "0";
        // }
        // path += std::to_string(x);
        // path += ".JPG";
    cv::Mat image = cv::imread(path,cv::IMREAD_GRAYSCALE);    //Import the image
    cv::Mat background_mask, foreground_mask;
    Watershed::background_mask(image, background_mask);
    Watershed::foreground_mask(image,foreground_mask,background_mask);
    cv::Mat result = original.clone();
    // Colorir a imagem para visualização
    //
    //

    // Create the ring-shaped matched filter
    cv::Mat src;
    image.copyTo(src);
    cv::medianBlur(src,src,3);
    Transformations::erosion(src,src,2,0,1);
    Transformations::dilation(src,src,2,0,1);
    Watershed::Standard_deviation(src,src,5);
    src.convertTo(src, CV_32F, 1.0 / 255.0);
    src.setTo(0,background_mask<1);
    cv::Mat center_mask = cv::Mat::zeros(image.size(),CV_8U);
    for(int g = 0; g < 3; g++){
        const int cell_radius = 16;
        int kernelSize = 53;         // Size of the ring filter kernel (choose an odd size)
        double sigma1 = 4.8 + g/1.0;         // Standard deviation for G1, higher standard deviation for higher radius
        std::cout << g << std::endl;
        double sigma2 = sqrt(2) * sigma1;  // Standard deviation for G2 (to get a ring effect)
        cv::Mat ringFilter = Watershed::createRingMatchedFilter(kernelSize, sigma1, sigma2);
        cv::Mat filteredImage;
        cv::filter2D(src, filteredImage, -1, ringFilter);
        filteredImage*=20;
        cv::Mat normalized;
        cv::normalize(filteredImage, normalized, 0, 255, cv::NORM_MINMAX);
        normalized.convertTo(filteredImage,CV_8UC1);
        std::vector<cv::Point2f> localMaxima;
        cv::goodFeaturesToTrack(filteredImage, localMaxima, 3000, 0.05, cell_radius);
        //Create empty image of size of image
        //Apply the points to the center_mask
        //look for closest point on the foreground_mask from each point
        //Loop need to be created to travers from point
        for (const auto& pt : localMaxima) {
            const int cell_radius_multiplier = 5;
            //Define limits for checking for other points in center mask
            const int upper_limit_x = pt.x + cell_radius < center_mask.rows ? pt.x + cell_radius : center_mask.rows-1;
            const int lower_limit_x = pt.x - cell_radius > 0 ? pt.x - cell_radius : 0;
            const int upper_limit_y = pt.y + cell_radius < center_mask.cols ? pt.y + cell_radius : center_mask.cols;
            const int lower_limit_y = pt.y - cell_radius > 0 ? pt.y - cell_radius : 0; 
            //Define limits for searching for element of foreground mask
            const int foreground_upper_limit_x = pt.x + cell_radius*cell_radius_multiplier < center_mask.rows ? pt.x + cell_radius*cell_radius_multiplier : center_mask.rows-1;
            const int foreground_lower_limit_x = pt.x - cell_radius*cell_radius_multiplier > 0 ? pt.x - cell_radius*cell_radius_multiplier : 0;
            const int foreground_upper_limit_y = pt.y + cell_radius*cell_radius_multiplier < center_mask.cols ? pt.y + cell_radius*cell_radius_multiplier : center_mask.cols;
            const int foreground_lower_limit_y = pt.y - cell_radius*cell_radius_multiplier > 0 ? pt.y - cell_radius*cell_radius_multiplier : 0;
            //Limit the foreground to work in the same time for down and up
            const int foreground_limit_x = abs(pt.x - foreground_lower_limit_x) > abs(pt.x - foreground_upper_limit_x) ? abs(pt.x - foreground_lower_limit_x) : abs(pt.x - foreground_upper_limit_x);
            const int foreground_limit_y = abs(pt.y - foreground_lower_limit_y) > abs(pt.y - foreground_upper_limit_y) ? abs(pt.y - foreground_lower_limit_y) : abs(pt.y - foreground_upper_limit_y);
            bool done=false;
            for(int x = pt.x; x < foreground_limit_x + pt.x && !done; x++) {
                for(int y = pt.y; y < foreground_limit_y + pt.y; y++) {
                    int diff_x = x - pt.x;
                    int diff_y = y - pt.y;

                    // Precompute coordinates
                    int new_x1 = pt.x - diff_x, new_x2 = pt.x + diff_x;
                    int new_y1 = pt.y - diff_y, new_y2 = pt.y + diff_y;

                    // Check bounds and draw if conditions are met
                    if(new_x1 >= 0 && new_y1 >= 0 &&
                    foreground_mask.at<uchar>(new_y1, new_x1) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x1,new_y1),center_mask,cell_radius)){
                            cv::circle(result, cv::Point(new_x1, new_y1), 3, cv::Scalar(0,55,255), -1);
                            cv::circle(center_mask, cv::Point(new_x1, new_y1), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                    else if(new_x1 >= 0 && new_y2 < background_mask.cols &&
                    foreground_mask.at<uchar>(new_y2, new_x1) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x1,new_y2),center_mask,cell_radius)){
                            cv::circle(result, cv::Point(new_x1, new_y2), 3, cv::Scalar(0,55,255), -1);
                            cv::circle(center_mask, cv::Point(new_x1, new_y2), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                    if(new_x2 < background_mask.rows && new_y1 >= 0 &&
                    foreground_mask.at<uchar>(new_y1, new_x2) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x2,new_y1),center_mask,cell_radius)){
                            cv::circle(result, cv::Point(new_x2, new_y1), 3, cv::Scalar(0,55,255), -1);
                            cv::circle(center_mask, cv::Point(new_x2, new_y1), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                    else if(new_x2 < background_mask.rows && new_y2 < background_mask.cols &&
                    foreground_mask.at<uchar>(new_y2, new_x2) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x2,new_y2),center_mask,cell_radius)){
                            cv::circle(result, cv::Point(new_x2, new_y2), 3, cv::Scalar(0,55,255), -1);
                            cv::circle(center_mask, cv::Point(new_x2, new_y2), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                }
            }

            // if(background_mask.at<int>(pt.x,pt.y) != 0){
            //     cv::circle(center_mask, pt, 1, cv::Scalar(255), -1); // Draw circles on maxima
            // }
        }
        // for (int i = 0; i < foreground_mask.rows; i++) {
        //     for (int j = 0; j < foreground_mask.cols; j++) {
        //         int label = foreground_mask.at<uchar>(i, j);
        //         int bg_label = background_mask.at<uchar>(i, j);
        //         // if(bg_label != 0){
        //         //     img_color.at<Vec3b>(i, j) = Vec3b(255, 125, 50); // Vermelho para bordas
        //         // }
        //         if (label != 0) { // Borda
        //             result.at<Vec3b>(i, j) = Vec3b(0, 0, 255); // Vermelho para bordas
        //         }
        //     }
        // }

        // for (const auto& pt : localMaxima) {
        //     if(background_mask.at<int>(pt.x,pt.y) != 0){
        //         cv::circle(result, pt, 2, cv::Scalar(255,0,255), -1); // Draw circles on maxima
        //     }
        // }
    }
    cv::imshow("src",src);
    cv::imshow("Local Maxima", result);
    cv::waitKey(0);

    // Draw the local maxima points
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    // // Perform dilation
    // cv::Mat dilated;
    // cv::dilate(filteredImage, dilated, kernel);
    // // Compare the original image with the dilated image
    // cv::Mat localMaxima;
    // cv::compare(filteredImage, dilated, localMaxima, cv::CMP_EQ);
    // cv::imshow("FFFF", localMaxima);
    // cv::waitKey(0);
    // // Optional: Filter out non-maxima or background (e.g., set threshold > 0)
    // cv::Mat mask;
    // mask = (filteredImage > x);

    // // Combine the mask with local maxima
    // cv::bitwise_and(localMaxima, mask, localMaxima);

    // // Display the result
    // cv::imshow("Local Maxima", localMaxima);
    // cv::waitKey(0);
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
    // cv::Mat markers = cv::Mat::zeros(img.size(), CV_32S);
     background_mask = (background_mask == 0);
    // markers.setTo(1,background_mask);
    // markers.setTo(2,foreground_mask);
    cv::Mat labels;
    cv::connectedComponents(center_mask,labels);
    cv::Mat markers = labels.clone(); // Start with connected components as markers
    markers.setTo(1, background_mask != 0); // Ensure background is labeled as 1
    markers.setTo(0, (background_mask == 0) & (center_mask == 0)); // Unknown regions are 0

    
    // 2 | 2 | 5 | 2.0 | 3
    cv::Mat img_2;
    img_wat.copyTo(img_2);
    Transformations::erosion(img,img_2,2,0,1);
    Transformations::dilation(img_2,img_2,2,0,1);
    cv::medianBlur(img_2,img_2,5);
    // Watershed::clache_medBlur(img_2,img_2,2.0,3);
    cv::cvtColor(img_2, img_wat, cv::COLOR_GRAY2BGR);
    cv::watershed(img_wat,markers);

    // Mat result;
    cv::cvtColor(image,result,cv::COLOR_GRAY2RGB);
    int count_green = 0;
    // img_wat.copyTo(result);
    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            int markerValue = markers.at<int>(i, j);
            if (markerValue == -1) {
                result.at<Vec3b>(i, j) = Vec3b(2, 0, 255); // Red for watershed boundaries
            }
            else if (markerValue == 1) {
                result.at<Vec3b>(i, j) = Vec3b(250, 100, 255); // Blue for background
            }
            else if (markerValue >= 2) {
                count_green += 1;
                result.at<Vec3b>(i, j) = Vec3b((markerValue % 5)*100,255,0);
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