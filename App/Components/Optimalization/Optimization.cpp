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
    const double resolution = 0.2;
    Transformations::square_and_resize(mask,int(resolution*mask.cols));
    mask.convertTo(this->user_mask_matrix, CV_32S);
    //I don't think I need to normalize it

}

void Optimalization::single_option_optimization(cv::Mat &img, AlgorithmOptions &options, std::string option_class, size_t option_index,bool uneven,int max_iterations, int min_start, double resolution){
    //Rescale the image to given ratio
    const int opt_size = int(img.cols * resolution);   //Material should be a square
    cv::Mat image, result;
    img.copyTo(image);
    Transformations::square_and_resize(image,opt_size);
    double precision = 0.0, new_precision = 0.0;
    //get the value of the option, and define max and minimum values.
    if(options.get_db_var(option_index,option_class) == AlgorithmOptions::MAX_DB){
        //Then we work with integers
        //With integers it is easier, just iterate over every value from 0 to 10
        simulate_watershed(image,result,options);
        int best_val = options.get_int_var(option_index,option_class);
        precision = calculate_intersection_over_union(result);
        for(int x = min_start; x < max_iterations; x++){
            if(uneven && x !=1){
                x+=1;
            }
            options.set_int_value(option_index, option_class, x);
            simulate_watershed(image,result,options);
            new_precision = calculate_intersection_over_union(result);
            // std::cout << "PRECISION: " << new_precision << std::endl;
            if(new_precision > precision){
                precision = new_precision;
                std:: cout << "Found new best value for " << option_index << " : " << x << "  || Precision: " << precision << std::endl;
                best_val = x;
            }
        }
        options.set_int_value(option_index, option_class, best_val);
    }
    else{

    }

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
    long int pixel_sum = 0;
    for(size_t x = 0; x < unique_in_user_matrix.size(); x++){
        std::pair<double,long int> result = this->calc_intersection_over_union_per_object(unique_in_user_matrix[x],image);
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

void Optimalization::sum_up_storage(std::map<long int,long int> &storage, int object_value){
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
std::pair<double,long int> Optimalization::calc_intersection_over_union_per_object(int object_id, cv::Mat &matrix){
    //Working in O(2(n^2)) time, where n is width=height of matrix
    cv::Mat mask;
    cv::compare(this->user_mask_matrix, object_id, mask, cv::CMP_EQ);
    if(mask.size() != matrix.size()){
        throw std::invalid_argument("Error, trying to compare user mask, and argument mask, they are different in size");
    }
    //Creating a storage for calculating intersection over union
    std::map<long int,long int> intersection_storage;
    std::map<long int,long int> union_storage;
    long int overall_matrix = 0;
    for(int x=0; x < mask.rows; x++){
        for(int y=0;y<mask.cols;y++){
            int val_mask = mask.at<uchar>(x,y);
            int val_matrix = matrix.at<uchar>(x,y);
            if(val_mask != 0){
                sum_up_storage(intersection_storage,val_matrix);
                overall_matrix += 1;
                continue;
            }
            sum_up_storage(union_storage,val_matrix);
        }
    }
    //Now, find the maximum value of intersection over union, here only the maximum value is in interest
    double max_val = 0.0;
    for(const auto &iter : intersection_storage){
        long int key = iter.first;
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
        long int intersec_value = iter.second;
        long int union_value = overall_matrix - intersec_value + to_add;
        double IoU = (double)intersec_value / union_value;
        if(IoU > max_val){
            max_val = IoU;
        }
    }
    return std::pair<double,long int>(max_val,overall_matrix);
}

void Optimalization::simulate_watershed(cv::Mat &img, cv::Mat &out, AlgorithmOptions &options){
    cv::Mat background_mask, foreground_mask, foreground_regions;
    Watershed::background_mask(img,background_mask,options.get_int_var(0,"BACKGROUND_MASK"),options.get_int_var(1,"BACKGROUND_MASK"),options.get_db_var(2,"BACKGROUND_MASK"),options.get_int_var(3,"BACKGROUND_MASK"),options.get_int_var(4,"BACKGROUND_MASK"),options.get_int_var(5,"BACKGROUND_MASK"),options.get_db_var(6,"BACKGROUND_MASK"),options.get_int_var(7,"BACKGROUND_MASK"));
    // //This will be even longer
    Watershed::foreground_regions(img,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_REGIONS"),options.get_int_var(1,"FOREGROUND_REGIONS"),options.get_db_var(2,"FOREGROUND_REGIONS"),options.get_int_var(3,"FOREGROUND_REGIONS"),options.get_db_var(4,"FOREGROUND_REGIONS"),options.get_int_var(5,"FOREGROUND_REGIONS"));
    // //And thiss will be the longest
    Watershed::foreground_mask(img,foreground_mask,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_MASK"), options.get_int_var(1,"FOREGROUND_MASK"), options.get_int_var(2,"FOREGROUND_MASK"), options.get_int_var(3,"FOREGROUND_MASK"), options.get_db_var(4,"FOREGROUND_MASK"), options.get_db_var(5,"FOREGROUND_MASK"),options.get_db_var(6,"FOREGROUND_MASK"), options.get_db_var(7,"FOREGROUND_MASK"), options.get_int_var(8,"FOREGROUND_MASK"), options.get_int_var(9,"FOREGROUND_MASK"), options.get_int_var(10,"FOREGROUND_MASK"), options.get_db_var(11,"FOREGROUND_MASK"), options.get_int_var(12,"FOREGROUND_MASK"));
    //And apply the watershed with masks and options
    Watershed::watershed_with_masks(img,out,background_mask,foreground_mask,options.get_int_var(0,"WATERSHED"),options.get_int_var(1,"WATERSHED"),options.get_int_var(2,"WATERSHED"),options.get_db_var(3,"WATERSHED"),options.get_int_var(4,"WATERSHED"));
}