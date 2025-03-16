#include "./Optimaliation.h"


void Optimalization::crop_save_image_sample(cv::Mat &out_img,std::vector<std::string> &images, std::string save_path,int scale){
    const std::string image_to_crop = images[rand() % images.size()];
    const int CROP_SIZE = 200;
    //Select random area on the image
    cv::Mat image = cv::imread(image_to_crop,cv::IMREAD_GRAYSCALE);
    int begin_x = rand() % (image.cols - CROP_SIZE);
    int begin_y = rand() % (image.rows - CROP_SIZE);
    //define region of interest
    cv::Rect roi(begin_x,begin_y,CROP_SIZE,CROP_SIZE);
    cv::Mat cropped = image(roi);
    Transformations::square_and_resize(cropped,CROP_SIZE*scale);
    cv::imwrite(save_path.c_str(),cropped);
    out_img = cropped;
}

void Optimalization::read_user_mask(std::string path_to_mask){
    cv::Mat mask = cv::imread(path_to_mask,cv::IMREAD_GRAYSCALE);
    mask.convertTo(this->user_mask_matrix, CV_32S);
    //I don't think I need to normalize it

}

double Optimalization::calculate_intersection_over_union(cv::Mat &image){
    //Check if user mask is specified
    if(this->user_mask_matrix.empty()){
        throw std::invalid_argument("Trying to calculate intersection without user mask specified");
    }
    //Check if image is specified
    if(image.empty()){
        throw std::invalid_argument("Argument image in intersection calculation is not specified");
    }
    //Detect how many unique objects are there in a user matrix
    std::vector<int> unique_in_user_matrix;
    this->find_unique_values(this->user_mask_matrix,unique_in_user_matrix);
    //Then, calculate the intersection per object, including it's size
    double val_sum = 0;
    int pixel_sum = 0;
    for(size_t x = 0; x < unique_in_user_matrix.size(); x++){
        std::pair<double,int> result = this->calc_intersection_over_union_per_object(unique_in_user_matrix[x],image);
        val_sum += result.first * result.second;
        pixel_sum += result.second;
    }
    return val_sum / pixel_sum;
}


//Private


void Optimalization::find_unique_values(cv::Mat &matrix, std::vector<int> &storage){
    for (int x=0; x < matrix.rows; x++){
        for(int y = 0; y < matrix.cols; y++){
            int value = matrix.at<int>(x,y);
            if(value == -1 || value == 0)continue;
            if(std::find(storage.begin(), storage.end(), value) == storage.end() ){
                storage.push_back(value);
            }
        }
    }
}

void Optimalization::sum_up_storage(std::map<int,int> &storage, int object_value){
    //Find if key already exists
    auto it = storage.find(object_value);
    if(it == storage.end()){
        storage.insert({object_value,1});
        return;
    }
    //If found
    it->second = it->second+1; 
}

//I KNOW THIS ALGORITHM CAN BE MORE OPTIMAL, BUT I DID IT THEY WAY I DID IT, SO SCREW IT
//I mean, it could be reduced from in final version O(p(2n^2)), where p=nr of objects in user_mask, to just O(2n^2) ...
std::pair<double,int> Optimalization::calc_intersection_over_union_per_object(int object_id, cv::Mat &matrix){
    //Working in O(2(n^2)) time, where n is width=height of matrix
    cv::Mat mask = this->user_mask_matrix == object_id;
    if(mask.size() != matrix.size()){
        throw std::invalid_argument("Error, trying to compare user mask, and argument mask, they are different in size");
    }
    //Creating a storage for calculating intersection over union
    std::map<int,int> intersection_storage;
    std::map<int,int> union_storage;
    int overall_matrix = 0;
    for(int x=0; x < mask.cols; x++){
        for(int y=0;y<mask.rows;y++){
            int val_mask = mask.at<int>(x,y);
            int val_matrix = matrix.at<int>(x,y);
            if(val_mask != 0){
                sum_up_storage(intersection_storage,val_matrix);
                overall_matrix += 1;
                continue;
            }
            sum_up_storage(union_storage,val_matrix);
        }
    }
    //Now, find the maximum value of intersection over union, here only the maximum value is in interest
    double max_val = 0;
    for(const auto &iter : intersection_storage){
        int key = iter.first;
        if(key == 1)continue; //Exclude background
        int to_add = -1;
        //Find the same key in union_storage
        auto union_val = union_storage.find(key);
        if(union_val == union_storage.end()){
            to_add = 0;
        }
        if(to_add == -1){
            to_add = union_val->second;
        }
        int intersec_value = iter.second;
        int union_value = overall_matrix - intersec_value + to_add;
        double IoU = (double)intersec_value / union_value;
        if(IoU > max_val){
            max_val = IoU;
        }
    }
    return std::pair<double,int>(max_val,overall_matrix);
}