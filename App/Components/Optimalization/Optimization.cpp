#include "./Optimaliation.h"


void Optimalization::crop_save_image_sample(cv::Mat &out_img,std::vector<std::string> &images, std::string save_path,int crop_size){
    const int SCALE = int(1 / this->resolution);
    const std::string image_to_crop = images[rand() % images.size()];   //Choosing random image from images vector
    const int CROP_SIZE = crop_size;
    //Selecting random area on the image, random begin_x and begin_y.
    cv::Mat image = cv::imread(image_to_crop,cv::IMREAD_GRAYSCALE);
    int begin_x = rand() % (image.cols - CROP_SIZE);
    int begin_y = rand() % (image.rows - CROP_SIZE);
    //define region of interest and crop the image
    cv::Rect roi(begin_x,begin_y,CROP_SIZE,CROP_SIZE);
    cv::Mat cropped = image(roi);
    //Resize the cropped image with scale
    Transformations::square_and_resize(cropped,CROP_SIZE*SCALE);
    cv::imwrite(save_path.c_str(),cropped);
    out_img = cropped;  //save to out image
}

void Optimalization::read_user_mask(std::string path_to_mask){
    cv::Mat mask = cv::imread(path_to_mask,cv::IMREAD_GRAYSCALE);
    Transformations::square_and_resize(mask,int(this->resolution*mask.cols));
    mask.convertTo(this->user_mask_matrix, CV_32S);
}

void Optimalization::start_optimization(cv::Mat &org_img, std::string mask_path, std::string options_path, std::string out_options_path, bool shuffle, size_t iterations, bool save_order, bool itter_shuffle, int ID){
    read_user_mask(mask_path);
    //Load the options to one single vector
    AlgorithmOptions options(options_path.c_str());
    //Order is to be changed and can be changed
    std::vector<opt_option> options_to_optimize;
    //options_to_optimize.push_back(opt_option("BACKGROUND_MASK",2,false,20,1,0.1)); //IGNORED, because BG mask works very good
    //options_to_optimize.push_back(opt_option("BACKGROUND_MASK",6,false,20,1,0.1)); //IGNORED, because BG mask works very good
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",8,false,20,10,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",9,false,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",12,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",2,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",4,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",3,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",0,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",1,true,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_REGIONS",5,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",4,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",5,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",6,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",1,false,20,1,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",11,false,20,0,0.1));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",2,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",3,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",0,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("FOREGROUND_MASK",1,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",0,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",2,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",4,true,20,1,0.2));
    options_to_optimize.push_back(opt_option("WATERSHED",1,true,20,1,0.1));
    //Important, set max cells number, normally it should be dependent on background extraction
    options.set_int_value(10,"FOREGROUND_MASK",int(this->resolution*0.1*options.get_int_var(10,"FOREGROUND_MASK")));
    double best_val = 0.0, val = 0.0;
    std::random_device rd;
    std::mt19937 g(rd());
    if(shuffle){
        std::shuffle(options_to_optimize.begin(), options_to_optimize.end(),g);
    }
    const int opt_size = int(org_img.cols * this->resolution);   //Material should be a square
    cv::Mat image;
    org_img.copyTo(image);
    Transformations::square_and_resize(image,opt_size);

    for(size_t y = 0; y < iterations; y++){
        std::cout << "--OPTIMIZATION-- CURRENT ITERATION: [" << y+1 << "/" << iterations << "].\n";
        for(size_t x = 0; x < options_to_optimize.size(); x++){
            std::cout << '\r' << std::flush;
            for(size_t i=0; i < options_to_optimize.size(); i++){
                std::cout << "[";
                if(i <= x){
                    std::cout << "*";
                }else{
                    std::cout << " ";
                }
                std::cout << "]";
            }
            std::cout << std::flush;
            if(x == options_to_optimize.size() - 1){ 
                std::cout << std::endl;  // Print newline after the last iteration
            }
            val = this->single_option_optimization(image, options,options_to_optimize[x].name,options_to_optimize[x].index,options_to_optimize[x].is_iter,options_to_optimize[x].max_iter,options_to_optimize[x].min_iter,options_to_optimize[x].resolution);
        }
        if(val > best_val){
            best_val = val;
            std::string order = "#ORDER OF OPTIONS:: \n#PRECISION: " + std::to_string(best_val) + "\n \n";

            if(save_order){
                for(size_t i = 0; i < options_to_optimize.size(); i++){
                    order += "#" + options_to_optimize[i].name + "    " + std::to_string(options_to_optimize[i].index) + "\n";
                }
            }
            if(ID != 0){
                std::string to_save = out_options_path.substr(0, out_options_path.find(".option")) + "0" + std::to_string(ID) + ".option";
                options.save_options_to_file(to_save.c_str(), order);
            }else{
                options.save_options_to_file(out_options_path.c_str(), order);
            }
        }
        if(itter_shuffle){
            std::shuffle(options_to_optimize.begin(), options_to_optimize.end(),g);
        }
    }
    //End of optimization
}

double Optimalization::single_option_optimization(cv::Mat &img, AlgorithmOptions &options, std::string option_class, size_t option_index,bool uneven,int max_iterations, int min_start, double resolution){
    //Rescale the image to given ratio
    cv::Mat image = img, result;
    // image.convertTo(image, CV_32S);
    double precision = 0.0, new_precision = 0.0;
    //get the value of the option, and define max and minimum values.
    if(options.get_db_var(option_index,option_class) == AlgorithmOptions::MAX_DB){
        //Then we work with integers
        //With integers it is easier, just iterate over every value from 0 to 10
        Watershed::execute_watershed(image,result,options);
        int best_val = options.get_int_var(option_index,option_class);
        precision = calculate_intersection_over_union(result);
        for(int x = min_start; x < max_iterations; x++){
            if(uneven && x !=1){
                x+=1;
            }
            options.set_int_value(option_index, option_class, x);
            Watershed::execute_watershed(image,result,options);
            new_precision = calculate_intersection_over_union(result);
            // std::cout << "PRECISION: " << new_precision << std::endl;
            if(new_precision > precision){
                precision = new_precision;
                // std:: cout << "Found new best value for " << option_index << " : " << x << "  -- Precision: " << precision << std::endl;
                best_val = x;
            }
        }
        options.set_int_value(option_index, option_class, best_val);
    }
    else{
        const int DEPTH = 2;    //Depth defines how deeply the algorithm goes
        double range_min = options.get_db_var(option_index,option_class) / 10;
        if(range_min < 0.01)range_min = 0.01;
        double range_max = options.get_db_var(option_index,option_class) + (max_iterations/(DEPTH*DEPTH));
        double mx_range = 0.0, ms_range = 0.0;    //ms_range will also be the flag
        double step = 0.1;
        Watershed::execute_watershed(image,result,options);
        double best_precision = calculate_intersection_over_union(result);
        double best_value = options.get_db_var(option_index, option_class);
        double current_reach_level = 10.0;
        for(int i = 0; i < DEPTH; i++){
            for(double x = range_min; x < range_max; x+=step){
                options.set_db_value(option_index, option_class, x);
                Watershed::execute_watershed(image,result,options);
                new_precision = calculate_intersection_over_union(result);
                // std::cout << "CALC: " << x << "PREC: " << new_precision << " INDEX: " << option_index << std::endl;
                if(new_precision > best_precision){
                    best_precision = new_precision;
                    ms_range = x-(0.90/current_reach_level);
                    mx_range = x+(0.90/current_reach_level);
                    best_value = x;
                    // std:: cout << "Found new best value for " << option_index << " : " << best_value << "  || Precision: " << best_precision << std::endl;
                }
            }
            if(mx_range == 0.0){
                mx_range = options.get_db_var(option_index,option_class) +(0.90/current_reach_level);
                ms_range = options.get_db_var(option_index,option_class) -(0.90/current_reach_level);
            }
            if(ms_range <= 0){
                ms_range = 0.01;
            }
            range_max = mx_range;
            range_min = ms_range;
            step = step/10;
            current_reach_level = current_reach_level*10;
        }
        options.set_db_value(option_index, option_class, best_value);
        precision = best_precision;
    }
    return precision;
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