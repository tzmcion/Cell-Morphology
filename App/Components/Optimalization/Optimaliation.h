/**
 * This file is used for options optimalization
 * For handlign the integration between the modules
 */

 #include "../Threading/Threads.h"
 #include "../Includes/includes.h"
 #include "../Structures/AlgorithmOptions.h"

 class Optimalization{
    //Class for handling Optimalization
    public:
        Optimalization();
        ~Optimalization();



    private:
        /** 
         * Function crops the image and saves it to the temp folder in provided name
         * @param scale Parameter to define the scale of the saving
        */
        void crop_save_image_sample(int scale=1);

        /**
         * Function aims to sequentially change single option to find the best match for user image
         */
        void single_option_optimization();

        /**
         * Function reads the mask saved in image and saves it in variable
         */
        void read_user_mask();

        cv::Mat user_mask_matrix; //OpenCV like connected components matrix for user mask
        AlgorithmOptions *options;  //Algorithm options

        enum Response{  //Possible responses in communication file
            SAMPLE_CH,
            START_OPT,
            AWAIT_USR,
            PAUSE_OPT //This one to do later
        };
 };