/**
 * This file is used for options optimalization
 * The optimalization process involves changing the options, and calculating watershed precision compared to goal.
 * It is done by comparing user_generated mask, where the marked regions are objects, with result of watershed with current set of options.
 * Therefore, each optimization calculates IoU, then settings with best IoU are saved and called optimized
 * Process is highly time consuming, and real difference before and after optimalization is ~5-10% more precision in segmentation
 */

 #pragma once

 #include "../Threading/Threads.h"
 #include "../Includes/includes.h"
 #include "../Structures/AlgorithmOptions.h"
 #include "../Transformations/Transformations.h"
 #include "../Watershed/Watershed.h"

 /**
  * Class Optimalization handles .aloptions file adaptation for specified patch of images.
  * The process chanhges the options, and improves the precission of segmentation.
  */
 class Optimalization{
    public:
        /**
         * Class Optimalization handles .aloptions file adaptation for specified patch of images.
         * The process chanhges the options, and improves the precission of segmentation.
         * 
         * @param resolution Resolution defines how much the sample is zoomed, it is involved in generating sample to create user mask, as well as in calculating watershed and optimizing
         * resolution defines how biig the scaling is when sending sample tu GUI, by dividing 1 by resolution, so for res=0.25, scale=4, for res=1, scale=1
         * resolution also defines how the user_mask is scaled down. These scaling need to be exactly the same
         */
        Optimalization(double resolution = 0.25){
            this->resolution = resolution;
        };
        ~Optimalization(){};

        /** 
         * Method crops the image and saves it to the provided path name
         * The crop coordinates are random, as well as the image, it is chosen randomly from images vector
         * After crop, the sample is scaled dependent on resolution specified in constructor. 
         * scale = 1/resolution
         * 
         * @return cropped and scaled image saved in save_path, and allocated in out_img
         * @param out_img destination mask of image
         * @param images vector of paths to images from which the crop sammple will be drawn
         * @param save_path path in which the file will be saved
         * @param crop_size size of crop, it is square with width=height=crop_size
        */
        void crop_save_image_sample(cv::Mat &out_img,std::vector<std::string> &images, std::string save_path,int crop_size = 150);

        /**
         * Method sequentially changes the values of a single option in AlgorithmOptions.
         * With that changes, it calculates IoU every iteration, and compares it.
         * When options configuration with better IoU is found, the options are
         * permanently changed to the new value.
         * Method ranges from min_start to max_iterations, with possible resolution changes
         * 
         * @returns function returns IoU value from best iteration, and permanently altered AlgorithmOptions object.
         * @param img image sample generated from AlgorithmOptions::crop_save_image_sample() on which the watershed is performed
         * @param options AlgorithmOptions read from .aloption file, which will be altered during optimization process
         * @param option_class class of option which will be optimized
         * @param option_index index of option which will be optimized
         * @param uneven Some options, which use kernel in calculations, can only accept uneven values. Set this to true if the option needs only uneven numbers (1,3,5 etc.)
         * @param max_iterations Maximum iterations which the optiom will undergo (works only for <int> options)
         * @param min_start Minumum value from which the option will start (works only for <int> options)
         * @param resolution Resolution defines how precise the optimization is. With less resolution, the processed image is schrunk, therefore obtaining higher speed, but less real precision on image
         */
        double single_option_optimization(cv::Mat &img, AlgorithmOptions &options, std::string option_class, size_t option_index, bool uneven = false, int max_iterations=50, int min_start = 1, double resolution = 0.25);

        /**
         * Method reads the mask saved in image and saves it in variable user_mask_matrix
         * 
         * @param path_to_mask path to image storing user mask
         */
        void read_user_mask(std::string path_to_mask);

        /**
         * Method starts the main optimization of option
         */
        void start_optimization(cv::Mat &org_img, std::string mask_path, std::string options_path, std::string out_options_path, bool shuffle = false, size_t iterations=5, bool save_order = false, bool itter_shuffle=false, int ID=0);

        /**
         * Method compares user mask defined using method Optimalization::read_user_mask()
         * 
         * @return percentage of mask (image) which is correctly covering the image
         * @param image mask generated from watershed algorithm
         */
        double calculate_intersection_over_union(cv::Mat &image);

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

        //resolution is scale
        double resolution;

        struct opt_option{
            opt_option(std::string name, int index, bool is_iter, int max_iter, int min_iter, double resolution){
                this->name = name;
                this->index = index;
                this->is_iter = is_iter;
                this->max_iter = max_iter;
                this->min_iter = min_iter;
                this->resolution = resolution;
            }
            std::string name;
            int index;
            bool is_iter;
            int max_iter;
            int min_iter;
            double resolution;
        };
 };