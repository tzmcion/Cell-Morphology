#include "./Tracking.h"

#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
//
//
//
void Tracking::radius_by_centers(std::vector<cv::Mat> &patches, const char* options_path, std::vector<int> &averages, double &min_val_calc, double &max_val_calc){
    //From options extract value of cell radius
    AlgorithmOptions options(options_path);
    const int cell_radius = options.get_int_var(8, "FOREGROUND_MASK");
    double avg_radius = 0;
    //To collect the data for distribution
    const double RADIUS_MIN = (double)cell_radius*0.2;
    const double MAX_RADIUS = 150.0; //RADIUS_MIN*50;
    size_t distr_resolution = 30;
    averages.resize(distr_resolution);
    std::fill(averages.begin(), averages.end(), 0);
    std::vector<double> radius_values;
    //Perform connected_components with stats then iterate over every found object
    for(size_t x = 0; x < patches.size(); x++){
        cv::Mat background_mask, foreground_mask, foreground_regions;
        Watershed::background_mask(patches[x],background_mask,options.get_int_var(0,"BACKGROUND_MASK"),options.get_int_var(1,"BACKGROUND_MASK"),options.get_db_var(2,"BACKGROUND_MASK"),options.get_int_var(3,"BACKGROUND_MASK"),options.get_int_var(4,"BACKGROUND_MASK"),options.get_int_var(5,"BACKGROUND_MASK"),options.get_db_var(6,"BACKGROUND_MASK"),options.get_int_var(7,"BACKGROUND_MASK"));
        // //This will be even longer
        Watershed::foreground_regions(patches[x],foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_REGIONS"),options.get_int_var(1,"FOREGROUND_REGIONS"),options.get_db_var(2,"FOREGROUND_REGIONS"),options.get_int_var(3,"FOREGROUND_REGIONS"),options.get_db_var(4,"FOREGROUND_REGIONS"),options.get_int_var(5,"FOREGROUND_REGIONS"));
        // //And thiss will be the longest
        Watershed::foreground_mask(patches[x],foreground_mask,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_MASK"), options.get_int_var(1,"FOREGROUND_MASK"), options.get_int_var(2,"FOREGROUND_MASK"), options.get_int_var(3,"FOREGROUND_MASK"), options.get_db_var(4,"FOREGROUND_MASK"), options.get_db_var(5,"FOREGROUND_MASK"),options.get_db_var(6,"FOREGROUND_MASK"), options.get_db_var(7,"FOREGROUND_MASK"), options.get_int_var(8,"FOREGROUND_MASK"), options.get_int_var(9,"FOREGROUND_MASK"), options.get_int_var(10,"FOREGROUND_MASK"), options.get_db_var(11,"FOREGROUND_MASK"), options.get_int_var(12,"FOREGROUND_MASK"));
        cv::Mat matrix, areas, centroids;
        const int numLabels = cv::connectedComponentsWithStats(foreground_mask, matrix, areas, centroids);
        //Execute for each object here
        for(int object = 1; object < numLabels; object++){
            cv::Point center(cvRound(centroids.at<double>(object,0)),cvRound(centroids.at<double>(object,1)));
            double radius = Tracking::get_closest_point(center,matrix,object,RADIUS_MIN,MAX_RADIUS);
            avg_radius += radius;
            radius_values.push_back(radius);
            
        }
        double radius_max_val = 0.0;
        for(size_t i = 0; i < radius_values.size(); i++){
            if(radius_values[i] > radius_max_val){
                radius_max_val = radius_values[i];
            }
        }
        radius_max_val = 150.0;
        for(size_t i = 0; i < radius_values.size(); i++){
            averages[Tracking::__vector_index_by_value(RADIUS_MIN,radius_max_val,radius_values[i],distr_resolution)] += 1;
        }
        min_val_calc = RADIUS_MIN;
        max_val_calc = radius_max_val;
        avg_radius /= numLabels;
    }
}
//
//
//
void Tracking::cout_distribution(std::vector<int> &distribution, int chart_height, double ch_min_val, double ch_max_val){
    //Printing by defined size of vector,height is set to be 10
    //Find the maximum value of distribution vector
    int max_val = 0;
    for(size_t i = 0; i < distribution.size(); i++){
        if(distribution[i] > max_val)max_val=distribution[i];
    }
    //Every time calculate normalized distribution
    std::cout << "|-";
    for(size_t y = 0; y < distribution.size(); y++){
        std::cout << "-------";
    }
    std::cout << "-|" << std::endl;
    for(uchar x = chart_height; x > 0; x--){
        std::cout << "| ";
        for(size_t y = 0; y < distribution.size(); y++){
            if(x <= std::ceil((distribution[y] / double(max_val)) * chart_height)){
                std::cout << "  ***  ";
            }
            else{
                std::cout << "       ";
            }
        }
        std::cout << " |" << std::endl;
    }
    std::cout << "| ";
    for(size_t y = 0; y < distribution.size(); y++){
        double val = std::round((ch_min_val + ch_max_val*(y/double(distribution.size()))) * 100.0) / 100.0;
        std::cout << '>';
        if (val >= 1000) {
            std::cout << std::setw(4) << std::fixed << std::setprecision(0) << val;
        } else if (val >= 100) {
            std::cout << std::setw(4) << std::fixed << std::setprecision(1) << val;
        } else if (val >= 10) {
            std::cout << std::setw(4) << std::fixed << std::setprecision(2) << val;
        } else {
            std::cout << std::setw(4) << std::fixed << std::setprecision(3) << val;
        }
        std::cout << ' ';
    }
    std::cout << " |" << std::endl;
    //Here display the exact amounts
    std::cout << "|  " << Colors::BRIGHT_RED;
    for(size_t y = 0; y < distribution.size(); y++){
        double val = double(distribution[y]);
        if (val >= 1000) {
            std::cout << std::setw(4) << std::fixed << std::setprecision(0) << val;
        } else if (val >= 100) {
            std::cout << std::setw(4) << std::fixed << std::setprecision(1) << val;
        } else if (val >= 10) {
            std::cout << std::setw(4) << std::fixed << std::setprecision(2) << val;
        } else {
            std::cout << std::setw(4) << std::fixed << std::setprecision(3) << val;
        }
        std::cout << "  ";
    }
    std::cout << Colors::RESET << "|" << std::endl << "|-";
    for(size_t y = 0; y < distribution.size(); y++){
        std::cout << "-------";
    }
    std::cout << "-|" << std::endl;
}
//
//
//
double Tracking::get_closest_point(cv::Point point, cv::Mat& matrix, int object_id, double min_radius, double max_radius){
    //Let's do it like this, go from point.x - radius, to point.x + radius, and if in it radius something will be found, then this radius will stay.
    //With y, it will be calculated based on x. Pitaghoras and (x-a)^2 + (y-b)^2 = r^2
    double current_radius = min_radius;
    for(;current_radius < max_radius; current_radius += 1.0){
        for(int x = point.x - ROUND_2_INT(current_radius); x < point.x + ROUND_2_INT(current_radius); x++){
            if(x < 0 || x >= matrix.cols)continue;
            int y = ROUND_2_INT(sqrt(pow(ROUND_2_INT(current_radius),2) - pow(x - point.x,2))); //This is nothing more than pythagoras...
            //Check left pixel if matrix is not equal to 0
            int left_y = point.y - y;
            if(!(left_y < 0)) 
                if(matrix.at<uchar>(left_y, x) != 0){
                    return sqrt(pow(x-point.x,2) + pow(y,2));
                }
            //Check right pixel
            int right_y = point.y + y;
            if(!(right_y >= matrix.rows))
                if(matrix.at<uchar>(right_y, x) != 0){
                    return sqrt(pow(x-point.x,2) + pow(y,2));
                }
        }
    }
    return max_radius;
}
//
//
//
