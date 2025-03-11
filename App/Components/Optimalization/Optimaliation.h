/**
 * This file is used for options optimalization
 * For handlign the integration between the modules
 */

 #include "../Threading/Threads.h"
 #include "../Includes/includes.h"
 #include "../Structures/AlgorithmOptions.h"
 #include "../Transformations/Transformations.h"

 class Optimalization{
    //Class for handling Optimalization
    public:
        Optimalization(){};
        ~Optimalization(){};
        /** 
         * Function crops the image and saves it to the temp folder in provided name
         * @param scale Parameter to define the scale of the saving
        */
        void crop_save_image_sample(cv::Mat &out_img,std::vector<std::string> &images, std::string save_path,int scale=3){
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

        /**
         * Function aims to sequentially change single option to find the best match for user image
         */
        void single_option_optimization(){};

        /**
         * Function reads the mask saved in image and saves it in variable
         */
        void read_user_mask(){};

        cv::Mat user_mask_matrix; //OpenCV like connected components matrix for user mask
        AlgorithmOptions *options;  //Algorithm options

        enum Response{  //Possible responses in communication file
            SAMPLE_CH,
            START_OPT,
            AWAIT_USR,
            PAUSE_OPT //This one to do later
        };
 };