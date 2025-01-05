#pragma once
#include "../Includes/includes.h"

using namespace cv;
using namespace std;

class BasicTransofrmations{
    public:
        /**
         * Performs opencv dilation
         * */
        static void dilation(Mat &inp, Mat &out, int size, int elem = 2, int iterations = 1){
            cv::Mat src;
            src = inp;
            int erosion_type = 0;
            if( elem == 0 ){ erosion_type = MORPH_RECT; }
            else if( elem == 1 ){ erosion_type = MORPH_CROSS; }
            else if( elem == 2) { erosion_type = MORPH_ELLIPSE; }
            Mat element = getStructuringElement( erosion_type, Size( 2*size + 1, 2*size+1 ),Point( size, size ) );
            dilate( src, out, element, Point(-1,-1), iterations);
        }

        /**
         *  Performs opencv erosion
         * */
        static void erosion(Mat &inp, Mat &out, int size, int erosion_elem, int iterations){
            // if(BasicTransofrmations::is_null<int>(inp,out,size))return;
            int erosion_type = 0;
            if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
            else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
            else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
            Mat element = getStructuringElement( erosion_type, Size( 2*size + 1, 2*size+1 ),Point( size, size ) );
            erode( inp, out, element, Point(-1,-1), iterations);
        }

        /**
         *  Applies a clache transformation on the image
         *  Clache makes the details on the image more highlighted
         * */
        static void clahe(Mat &inp, Mat &out, double force){
            if(BasicTransofrmations::is_null<double>(inp,out,force))return;
            cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
            clahe->setClipLimit(force);
            cv::Mat claheImg;
            clahe->apply(inp, out);
        }

        /**
         *  Fires Gaussian blur on the image, then median Blur on the image
         * */
        static void double_blur(Mat &inp, Mat &out, int blur_force, int mean_force){
            BasicTransofrmations::is_image(inp);
            if((mean_force == blur_force) == 0){
                out = inp;
                return;
            }
                if(blur_force != 0)
                cv::GaussianBlur(inp, out, cv::Size(blur_force, blur_force), 0);
                if(mean_force != 0)
                cv::medianBlur(inp,out,mean_force);
        }

    private:
    /**
     *  Check if provided value is 0
     *  If is 0, then sets output to input and quits
     * */
    template <typename T>
    static bool is_null(Mat &inp, Mat &out, T check_val){
        BasicTransofrmations::is_image(inp);
        if(check_val == 0){
            out = inp;
            return true;
        }
    }

    protected:
    /**
     *  Function check if provide variable is image, and end quits program if is not
     * */
    static void is_image(cv::Mat &img, std::string path="certain_path"){
        if(img.empty()){
            const std::string msg = "Could not open the image on path: " + path;
            throw std::invalid_argument(msg);
        }
    }
};