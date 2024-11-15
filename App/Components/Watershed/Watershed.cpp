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
    cv::medianBlur(image,image,11);
    Watershed::Standard_deviation(image,image,3);
    Watershed::clache_medBlur(image,image,2.0,13);
    Watershed::clache_medBlur(image,image,2.0,9);
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

void Watershed::SD_antisotropic(cv::Mat &src, cv::Mat &dst, int SD_kernel, int as_iterations,double as_time){
    cv::Mat image;
    Watershed::Standard_deviation(src,image,3);
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
void Watershed::Standard_deviation(cv::Mat &src, cv::Mat &dst, int kernel){
    cv::Mat image, image_float, squared_image, squared_mean, mean, std_dev_image,std_dev_display;
    src.convertTo(image_float,CV_32F);
    //To calculate standard deviation, needed is to peform root sqare of a variation
    //σ^2 = Var(X,Y) = E(X^2) - (E(X))^2
    //σ = sqrt(σ^2)...
    cv::Mat mat_kernel = cv::Mat::ones(cv::Size(kernel, kernel), CV_32F) / (kernel * kernel);
    cv::pow(image_float,2,squared_image);
    cv::filter2D(image_float,mean,-1,mat_kernel);
    cv::multiply(mean,mean,mean);   //E(X)^2
    cv::filter2D(squared_image,squared_mean,-1,mat_kernel); //E(X^2)
    cv::sqrt(squared_mean - mean,std_dev_image);    //sqrt(Var(X,Y))
    cv::normalize(std_dev_image,std_dev_display,0,255,cv::NORM_MINMAX,src.type());
    dst = std_dev_display;
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
    cv::Mat minima_mask = (img < Transformations::image_brightnes(img)/10*15);

    // Step 3: Detect local minima within the mask
    // Define a kernel size for the minimum search (3x3 kernel used here)
    cv::Mat eroded;
    cv::dilate(img, eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

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

//
//
//
double Watershed::calculate_avg_radius(cv::Mat &src, int sure_min_radius, int sure_maximum_radius){
    return 2.2;
}
//
//
//
cv::Mat Watershed::createGaussianKernel(int size, double sigma){
    cv::Point center(size/2,size/2);

    cv::Mat kernel(size, size, CV_32F);
    double sigma2 = sigma * sigma;

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            double dx = x - center.x;
            double dy = y - center.y;
            double distance2 = sqrt(dx * dx + dy * dy) / (size/2) + 1;
            kernel.at<float>(x,y) = pow(size/2 - distance2,sigma);
        }
    }

    return kernel / cv::sum(kernel)[0] * 5;
}

cv::Mat Watershed::GaussianKernel(int size, double sigma){
    cv::Point center(size/2,size/2);

    cv::Mat kernel(size, size, CV_32F);
    double sigma2 = sigma * sigma;
    double constant = 1.0 / (2.0 * CV_PI * sigma2);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            double dx = x - center.x;
            double dy = y - center.y;
            double distance2 = dx * dx + dy * dy;
            kernel.at<float>(y, x) = constant * std::exp(-distance2 / (2 * sigma2));
        }
    }

    return kernel;
}
//
//
//
cv::Mat Watershed::createRingMatchedFilter(int size, double s1, double s2){
    // Create two Gaussian kernels G1 and G2
    cv::Mat G1 = Watershed::GaussianKernel(size, s1);
    cv::Mat G2 = Watershed::GaussianKernel(size, s2);

    cv::Mat G = Watershed::createGaussianKernel(size,s1);

    // Construct the ring filter based on the condition
    cv::Mat ringFilter(size, size, CV_32F);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Apply condition: if G1(x) < G2(x), keep G1(x), otherwise set to 0
            ringFilter.at<float>(i, j) = G1.at<float>(i, j) < G2.at<float>(i,j) ? G.at<float>(i,j) : 0.0f;
        }
    }

    return ringFilter;
}