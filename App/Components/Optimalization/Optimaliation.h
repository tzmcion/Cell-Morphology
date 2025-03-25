#pragma once
/**
 * This file is used for options optimalization
 * For handlign the integration between the modules
 */

 #include "../Threading/Threads.h"
 #include "../Includes/includes.h"
 #include "../Structures/AlgorithmOptions.h"
 #include "../Transformations/Transformations.h"
 #include "../Watershed/Watershed.h"

 class Optimalization{
    //Class for handling Optimalization
    public:
        Optimalization(){};
        ~Optimalization(){};
        /** 
         * Function crops the image and saves it to the temp folder in provided name
         * @param scale Parameter to define the scale of the saving
        */
        void crop_save_image_sample(cv::Mat &out_img,std::vector<std::string> &images, std::string save_path,int scale=3);

        /**
         * Function aims to sequentially change single option to find the best match for user image
         */
        void single_option_optimization(cv::Mat &img, AlgorithmOptions &options, std::string option_class, size_t option_index, bool uneven = false, int max_iterations=50, int min_start = 1, double resolution = 0.2);

        /**
         * Function reads the mask saved in image and saves it in variable
         */
        void read_user_mask(std::string path_to_mask);

        /**
         * Function compares user mask to the material mask specified
         */
        double calculate_intersection_over_union(cv::Mat &image);

        //Function runs the watershed segmentation with given options and returns a mask
        void simulate_watershed(cv::Mat &img, cv::Mat &out, AlgorithmOptions &options);

    private:
        
        //Function finds unique values in matrix and stores it in a vector
        void find_unique_values(cv::Mat &matrix, std::vector<int> &storage);

        //Function adds one, or creates a new record with a certain object_id
        void sum_up_storage(std::map<long int,long int> &storage, int object_value);

        /**
         * This function calculates maximum intersection over union on object (because many objects can overlap the user matrix)
         * returns pair, with first being max IoU, and second is the size of the object
         * @param object_id object id in USER MASK
         * @param matrix image/mask given to compare with USER MASK
         */
        std::pair<double,long int> calc_intersection_over_union_per_object(int object_id, cv::Mat &matrix);

        cv::Mat user_mask_matrix; //OpenCV like connected components matrix for user mask
        AlgorithmOptions *options;  //Algorithm options

        enum Response{  //Possible responses in communication file
            SAMPLE_CH,
            START_OPT,
            AWAIT_USR,
            PAUSE_OPT //This one to do later
        };
 };