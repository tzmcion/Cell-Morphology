#include "./Watershed.h"

/*
    Public functions
*/

void Watershed::background_mask(cv::Mat &src, cv::Mat &dst_mask){
    cv::Mat image;
    src.copyTo(image);
    Watershed::SD_antisotropic(image,image);
    Watershed::hysteresisThresholding(image,image,25,80,0);
    Watershed::white_inpaint_holes(image,image,int(src.cols / 3));
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::erode(image,dst_mask,kernel,cv::Point(-1,-1),1);
}
//
//
//
void Watershed::foreground_mask(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &bg_mask){
    cv::Mat image, mask;
    src.copyTo(image);
    Watershed::clache_medBlur(image,image,3.0,5);
    Watershed::calc_local_extremes(image,mask);
    for (int i = 0; i < bg_mask.rows; i++) {
        for (int j = 0; j < bg_mask.cols; j++) {
            int label = bg_mask.at<uchar>(i, j);
                if (label == 0) { // Borda
                    mask.at<uchar>(i, j) = 0;
                }
            }
    }
    mask = (mask > 0) * 255;
    dst_mask = mask;
}

/*
    Private functions
*/

void Watershed::SD_antisotropic(cv::Mat &src, cv::Mat &dst, int SD_kernel, int as_iterations,double as_time, bool AN){
    cv::Mat image;
    //Converting image to higher precision
    cv::Mat image_float;
    src.convertTo(image_float, CV_32F);
    //Compute the square and local min
    cv::Mat mean;
    cv::blur(image_float, mean, cv::Size(SD_kernel, SD_kernel));
    cv::Mat squared_image, squared_mean;
    cv::pow(image_float, 2, squared_image);
    cv::blur(squared_image, squared_mean, cv::Size(SD_kernel, SD_kernel));
    // Calculate local variance and standard deviation part
    cv::Mat local_variance = squared_mean - mean.mul(mean);
    cv::Mat std_dev_image;
    cv::sqrt(local_variance, std_dev_image);
    // Normalize the result to 8-bit image for display
    cv::Mat std_dev_display;
    cv::normalize(std_dev_image, std_dev_display, 0, 255, cv::NORM_MINMAX, CV_8U);
    image = std_dev_display;
    if(!AN){
        dst = image;
        return;
    }

    // Convert to color for anisotropicDiffusion
    cv::Mat color_image;
    cv::cvtColor(image, color_image, cv::COLOR_GRAY2BGR);
    const float K = 10.0;  
    cv::Mat diffused_img, opening;
    cv::ximgproc::anisotropicDiffusion(color_image,diffused_img,as_time,K,as_iterations);
    //Convert back to grayscale and return
    cv::cvtColor(diffused_img,image,cv::COLOR_BGR2GRAY);
    dst = image;
}
//
//
//
void Watershed::clache_medBlur(cv:: Mat &src, cv::Mat &dst, double clache_limit, int kernel){
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clache_limit); // Adjust clip limit for different effects
    cv::Mat image = src;
    clahe->apply(src, image);
    cv::medianBlur(image,image,kernel);
    Transformations::opening(dst,image,'O',3);
    dst = image;
}
//
//
//
void Watershed::hysteresisThresholding(cv::Mat &src, cv::Mat &dst, int min_threshold, int max_threshold, int min_area){
    // Initialize the output image and set to full black
    cv::Mat output;
    output = cv::Mat::zeros(src.size(), CV_8U);
    // Create a mask where strong pixels are marked
    cv::Mat strong_pixels, weak_pixels;
    if(max_threshold <= 0){
        double otsu = cv::threshold(src, strong_pixels, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        weak_pixels = (src >= min_threshold) & (src < otsu); //pixels which are Min < x < Max;
    }else{
        strong_pixels = (src >= max_threshold);  
        weak_pixels = (src >= min_threshold) & (src < max_threshold); //pixels which are Min < x < Max;
    }
    // Copy strong pixels to the output directly
    output.setTo(255, strong_pixels);
    //Filter out to small areas using connected components and opencv
    if(min_area > 1){
        cv::Mat labels, stats, centroids;
        int numComponents = cv::connectedComponentsWithStats(strong_pixels, labels, stats, centroids);
        for (int i = 1; i < numComponents; i++) {
            int area = stats.at<int>(i, cv::CC_STAT_AREA);   
            if (area < min_area) {
                strong_pixels.setTo(0, labels == i); //delete pixels which are not meeting the criteria
            }
        }
    }
    //Queue strong pixels which will be checked. 
    std::queue<cv::Point> pixels_to_check;
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            if (strong_pixels.at<uchar>(y, x)) {
                pixels_to_check.push(cv::Point(x, y));
            }
        }
    }
    //Run the queue and check 8 pixels in each strong pixel. If found, then continue the flooding process and do not delete that pixel from queue (continue with the same pixel)
    while (!pixels_to_check.empty()) {
        cv::Point p = pixels_to_check.front();
        pixels_to_check.pop();
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = p.x + dx;
                int ny = p.y + dy;
                if (nx >= 0 && ny >= 0 && nx < src.cols && ny < src.rows && weak_pixels.at<uchar>(ny, nx)) {
                    output.at<uchar>(ny, nx) = 255;
                    pixels_to_check.push(cv::Point(nx, ny));
                    weak_pixels.at<uchar>(ny, nx) = 0;
                }
            }
        }
    }
    dst = output;
}
//
//
//
void Watershed::hysteresisThresholding(cv::Mat &src, cv::Mat &dst, int min_threshold, int min_area){
    Watershed::hysteresisThresholding(src,dst,min_threshold,-1,min_area);
}
//
//
//
void Watershed::calc_local_extremes(cv::Mat &src, cv::Mat &dst_mask){
        // Step 1: Convert to 8-bit if needed
        // cv::normalize(img,img_stretched,0,255,cv::NORM_MINMAX);
    cv::Mat img_thresholded;
    cv::Mat minimo, img;
    src.copyTo(img);
    cv::Mat minima_mask = (img < Transformations::image_brightnes(img)/10*9);

    // Step 3: Detect local minima within the mask
    // Define a kernel size for the minimum search (3x3 kernel used here)
    cv::Mat eroded;
    cv::dilate(img, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

    // Compare original image to eroded image within the mask to find local minima
    cv::Mat sure_bg;
    cv::Mat local_minima = (img == eroded) & minima_mask;

    cv::Mat markerLabels;
    cv::connectedComponents(local_minima, markerLabels);
    markerLabels = markerLabels + 1; // Para que fundo seja 1 e não 0
    markerLabels.convertTo(markerLabels,CV_32S);

    // Optional: Visualize local minima (mark minima in a copy of the original image)
    dst_mask = local_minima;
}
//
//
//
void Watershed::white_inpaint_holes(cv::Mat &src, cv::Mat &dst, int max_area){
    cv::Mat output;
    src.copyTo(output);
    cv::Mat mask = (src < 1);
    cv::Mat labels, stats, centroids;
    int numComponents = cv::connectedComponentsWithStats(mask, labels, stats, centroids);
    for (int i = 1; i < numComponents; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);   
        if (area < max_area) {
            output.setTo(255, labels == i); //delete pixels which are not meeting the criteria
        }
    }
    dst = output;
}
//
//
//
double Watershed::calculate_avg_radius(cv::Mat &src, int sure_min_radius, int sure_maximum_radius){
    return 2.2;
}