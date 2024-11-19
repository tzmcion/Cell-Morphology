#include "./Watershed.h"

/*
    Public functions
*/

void Watershed::background_mask(cv::Mat &src, cv::Mat &dst_mask){
    const int SD_kernel = 3;
    const int as_iterations = 3;
    const double as_time = 0.1;
    const int hs_min = 25;
    const int hs_max = 80;
    const int hs_min_area = 0;
    const int max_hole_size = int(src.cols/3);
    const int erosion_size = 1;
    cv::Mat image;
    Watershed::Standard_deviation(src,image, SD_kernel);
    Watershed::Anisotropic(image,image,as_iterations, as_time);
    Watershed::hysteresisThresholding(image,image,hs_min,hs_max,hs_min_area);
    Watershed::white_inpaint_holes(image,image,max_hole_size);
    Transformations::erosion(image,dst_mask,erosion_size,0,1);
}
//
//
//
void Watershed::foreground_regoins(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &bg_mask){
    const int blur_kernel = 3;
    const int morph_opening_kernel = 3;
    const double clache_force = 2.0;
    const int second_blur_kernel = 3;
    const double minima_threshold = 0.92;
    const int minima_kernel = 1;
    cv::Mat image,mask;
    //Algorythm work
    cv::medianBlur(src,image,blur_kernel);
    Watershed::clache(image,image,clache_force);
    cv::medianBlur(image,image,second_blur_kernel);
    Transformations::opening(image,image,'O',morph_opening_kernel);
    Watershed::calc_local_minima(image,mask, minima_threshold, minima_kernel);
    //Remove background
    for (int i = 0; i < bg_mask.rows; i++) {
        for (int j = 0; j < bg_mask.cols; j++) {
            if (bg_mask.at<uchar>(i,j) == 0)
                mask.at<uchar>(i, j) = 0;
        }
    }
    dst_mask = mask * 255;
}
//
//
//
void Watershed::foreground_mask(cv::Mat &src, cv::Mat &dst_mask, cv::Mat &foreground_regions, cv::Mat &sure_background){
    const int blur_size = 3;
    const int dil_er_size = 2;
    const int SD_kernel_size = 5;
    const int iterations = 3;
    const double sigma_min = 4.0;
    const double sigma_max = 5.0;
    const double sigma_iterator = 1;
    const double sigma_multiplier = sqrt(2);
    const int cell_radius = 14;
    const int filter_kernel_size = cell_radius*3;
    const int good_features_max_corners = 3000;
    const double good_features_quality = 0.05;
    const int cell_radius_multiplier = 5;
    cv::Mat center_mask = cv::Mat::zeros(src.size(),CV_8U);
    cv::Mat img, filtered_image, normalized, test;
    cv::medianBlur(src,img,blur_size);
    Transformations::erosion(img,img,dil_er_size,0,1);
    Transformations::dilation(img,img,dil_er_size,0,1);
    Watershed::Standard_deviation(img,img,SD_kernel_size);
    img.convertTo(img, CV_32F, 1.0 / 255.0);
    img.setTo(0,sure_background<1);
    // //
    // cv::cvtColor(src, test, cv::COLOR_GRAY2BGR);
    // //
    for(double a = sigma_min; a <= sigma_max; a+= sigma_iterator){
        const cv::Mat ring_filter = Watershed::createRingMatchedFilter(filter_kernel_size, a, a*sigma_multiplier);
        cv::filter2D(img, filtered_image, -1, ring_filter);
        cv::normalize(filtered_image*20, normalized, 0, 255, cv::NORM_MINMAX);
        normalized.convertTo(filtered_image,CV_8UC1);
        std::vector<cv::Point2f> local_maxima;
        cv::goodFeaturesToTrack(filtered_image, local_maxima, good_features_max_corners, good_features_quality, cell_radius);
        for (const auto& pt : local_maxima) {
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
                    foreground_regions.at<uchar>(new_y1, new_x1) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x1,new_y1),center_mask,cell_radius)){
                            cv::circle(center_mask, cv::Point(new_x1, new_y1), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                    else if(new_x1 >= 0 && new_y2 < center_mask.cols &&
                    foreground_regions.at<uchar>(new_y2, new_x1) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x1,new_y2),center_mask,cell_radius)){
                            cv::circle(center_mask, cv::Point(new_x1, new_y2), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                    if(new_x2 < center_mask.rows && new_y1 >= 0 &&
                    foreground_regions.at<uchar>(new_y1, new_x2) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x2,new_y1),center_mask,cell_radius)){
                            cv::circle(center_mask, cv::Point(new_x2, new_y1), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                    else if(new_x2 < center_mask.rows && new_y2 < center_mask.cols &&
                    foreground_regions.at<uchar>(new_y2, new_x2) != 0) {
                        if(!Watershed::is_object_in_radius(cv::Point(new_x2,new_y2),center_mask,cell_radius)){
                            cv::circle(center_mask, cv::Point(new_x2, new_y2), 1, cv::Scalar(255), -1);
                            done = true;
                            break;
                        }
                    }
                }
            }
            // for(int x = 0; x < test.rows; x++){
            //     for(int y = 0; y < test.cols; y++){
            //         if(center_mask.at<uchar>(x,y) != 0){
            //             cv::circle(test, cv::Point(y, x), 1, cv::Scalar(0,255,0), -1);
            //         }
            //     }
            // }
            // cv::imshow("MASK",test);
            // cv::waitKey(1);
        }
    }
    dst_mask = center_mask;
}

/*
    Private functions
*/

void Watershed::Anisotropic(cv::Mat &src, cv::Mat &dst, int as_iterations,double as_time){
    const double K = 10.0;
    cv::Mat image;
    cv::Mat color_image;
    cv::cvtColor(src, color_image, cv::COLOR_GRAY2BGR);
    cv::Mat diffused_img, opening;
    cv::ximgproc::anisotropicDiffusion(color_image,diffused_img,as_time,K,as_iterations);
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
void Watershed::clache(cv:: Mat &src, cv::Mat &dst, double clache_limit){
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clache_limit); // Adjust clip limit for different effects
    cv::Mat image = src;
    clahe->apply(src, image);
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
void Watershed::calc_local_minima(cv::Mat &src, cv::Mat &dst_mask, double mean_multiplicator, int kernel){
    cv::Mat img, dilated;
    src.copyTo(img);
    //Get the mask of the image after threshold
    cv::Mat minima_mask = (img < Transformations::image_brightnes(img)*mean_multiplicator); 
    //Dilate the original image
    cv::dilate(img, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernel, kernel)));
    //Localize minima
    cv::Mat local_minima = (img == dilated) & minima_mask;
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
double Watershed::avg_radius_from_mask(cv::Mat &src, int sure_min_radius, int sure_maximum_radius){
    return 2.2;
}
//
//
//
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

    // Construct the ring filter based on the condition
    cv::Mat ringFilter(size, size, CV_32F);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Apply condition: if G1(x) < G2(x), keep G1(x), otherwise set to 0
            ringFilter.at<float>(i, j) = G1.at<float>(i, j) < G2.at<float>(i,j) ? G1.at<float>(i,j) : 0.0f;
        }
    }

    return ringFilter;
}
//
//
//
bool Watershed::is_object_in_radius(cv::Point p, cv::Mat &surface, int radius){
    const int col_x = p.x;
    const int row_y = p.y;
    const int start_x = p.x - radius > 0 ? p.x - radius : 0;
    const int start_y = p.y - radius > 0 ? p.y - radius: 0;
    for(int x = start_x; x < x+radius && x < surface.cols; x++){
        for(int y = start_y; y < y+radius && y < surface.rows; y++){
            if(surface.at<uchar>(y,x) != 0){
                //check if distance matches the radius
                const int d_x = abs(p.x - x);
                const int d_y = abs(p.y - y);
                const int distance = sqrt(pow(d_x,2) + pow(d_y,2));
                if(distance < radius)return true;
            }
        }
    }
    return false;
}