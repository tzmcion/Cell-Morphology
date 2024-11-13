#pragma once

#include "./Basic_Transformations.h"
#include "../Structures/Structures.h"

class Transformations:BasicTransofrmations{
    public:
    /*Function returns average brightnes of the image*/
    static double image_brightnes(cv::Mat &img);

    /*Function returns average brightness on the image excluding dark regions */
    static double image_brightnes(cv::Mat &img, double thr_to_exclude);

    /*Function alters the image brightnes within the file (opens and saves it)*/
    static void alter_brightnes(cv::Mat &img, double brightnes);

    /**
     *  Function performes Morphological opening, the operation can be:
     *  operation='O' -> Morphological Opening
     *  operation='C' -> Morphological Close
     *  operation='W' -> White Top-Hat
     *  operation='B' -> Black Top-Hat
     *  operation='G' -> Gradient
     * */
    static void opening(cv::Mat &image,cv::Mat src, char operation, int kernelSize=5);

    /**
     *  Function removes dots using connected components, can be removed by size and acceptance threshold
     *  @param threshold_black Threshold value for detecting the object as removable (ratio -- mean/25 * threshold_black)
     *  @param threshold_size size of detected object being to big to be counted as noise
     *  @param kern_s_dil_init size of initial dilation, before the painting of gray over the detected object 
     *  @param kern_s_dil_sec size of second dilation, after the painting of gray over the detected object
     *  @param inpaint_size size of kernel of inpaint, inpaint merges the objects with background
     *  @param type type of inpaint algorithm, 1 or 0
     * */
    static void dots_remove(cv::Mat &img, int threshold_black=15, int threshold_size=50, int kern_s_dil_init = 3, int kern_s_dil_sec = 5, int inpaint_size = 5, int type = 1, bool display_changes = false);

    /**
     *  Function normalizes brightnes of the image
     *  @param img cv.Mat imag3
     *  @param max_radius Maximum radius of the mean brightness of the area
     * */
    static void norm_brightnes(cv::Mat &img, int max_radius=50, int alter_radius=200, int threshold=30);

    /**
     * Function normalizes brightness on the image and thrives with it to the goal brightness
     * @param img cv.Mat image
     * @param goal_brightness goal brightness to which the image will thrive
     * */
    static void norm_brightnes(cv::Mat &img, double goal_brightness, int max_radius=50, int alter_radius=200, int threshold=30);

    /**
     *  Function crops the image to be a square, calculating from the middle
     *  @param img CV.Mat image to alter
     *  @param d_size desired size of image (width)
     * */
    static void square_and_resize(cv::Mat &img, int d_size = 1000);

    private:

    /**
     *  Function validates values for function dots_remove and quits the probram if values are out of bonds
     * */
    static void data_validation_dots_remove(int threshold_black, int threshold_size, int ker1, int ker2, int inp, int type){
        std::string message = "ERROR for algorithm of dots_remove provided value(s): ";
        std::string check = message;
        if(threshold_black <= 0 || threshold_black > 50){
            message += "* Threshold: ";
            message += threshold_black;
            message += " While the range is <int>[1, 50] \n";
        }
        if(threshold_size <= 0){
            message += "* area_size: ";
            message += threshold_size;
            message += " While the range is <int>[1, +inf)\n";
        }
        if(ker1 <= 0 || ker1 >= 8){
            message += "* Kernel_size_dilation_initial: ";
            message += ker1;
            message += " While the range is <int>[1, 7]\n";
        }
        if(ker2 <= 0 || ker2 >= 8){
            message += "* Kernel_size_dilation_second: ";
            message += ker2;
            message += " While the range is <int>[1, 7]\n";
        }
        if(inp <= 0 || inp >= 8){
            message += "* inpaint_size: ";
            message += inp;
            message += " While the range is <int>[1,7]\n";
        }
        if(type != 0 && type != 1){
            message += "* inpaint_type: ";
            message += type;
            message += " While this value is binarry <int>[0,1]";
        }
        if(check != message){
            throw std::invalid_argument(message);
        }
    }

    /**
     *  Function return mean of brightnes of circle on the image
     * */
    static double get_mean_of_circle(cv::Mat &img, cv::Point center, int radius = 50){
        //literally the same code as in alter_ without altering
                const int rows=img.rows, cols=img.cols;
        const int x_max = std::min(rows,center.x + radius);
        const int x_min = std::max(0,center.x - radius);
        const int y_max = std::min(cols,center.y + radius);
        const int y_min = std::max(0,center.y - radius);
        //Now, in a lop calculate distance
        //If distance from the center is less than radius, then the pixel is a part of the wheel
        //And will be altered
        int count = 0;
        double sum = 0;
        for(int x = x_min; x < x_max; x++){
            for(int y = y_min; y < y_max; y++){
                const int distance = std::floor(std::sqrt(std::pow(x - center.x,2) + std::pow(y - center.y,2)));
                if(distance <= radius){
                    sum += img.at<cv::uint8_t>(x,y);
                    count += 1;
                }
            }
        }
        return sum/count;
    }

    /**
     *  Function alters brightnes on a circle
     * */
    static void alter_mean_of_circle(cv::Mat &img, cv::Point center, int goal_brightness, int radius=50, int threshold_val = 20){
        //First, calculate boundries
        const int rows=img.rows, cols=img.cols;
        const int x_max = std::min(rows,center.x + radius);
        const int x_min = std::max(0,center.x - radius);
        const int y_max = std::min(cols,center.y + radius);
        const int y_min = std::max(0,center.y - radius);
        //Now, in a lop calculate distance
        //If distance from the center is less than radius, then the pixel is a part of the wheel
        //And will be altered
        int count = 0;
        double sum = 0;
        std::vector<cv::Point> points;
        std::vector<double> influence;
        for(int x = x_min; x < x_max; x++){
            for(int y = y_min; y < y_max; y++){
                const int distance = std::floor(std::sqrt(std::pow(x - center.x,2) + std::pow(y - center.y,2)));
                if(distance < radius){
                    if(abs(goal_brightness - img.at<float>(x,y)) > threshold_val){
                        cv::Point ap_point(x,y);
                        influence.push_back(double(radius - distance) / radius);
                        points.push_back(ap_point);
                        continue;
                    }
                    sum += img.at<float>(x,y);
                    count += 1;
                    cv::Point ap_point(x,y);
                    influence.push_back(double(radius - distance) / radius);
                    points.push_back(ap_point);
                }
            }
        }
        double ajd_mean = goal_brightness - sum/count;
        if(!(sum > 0) )return;
        //Influence size must be same size as points
        for(size_t i = 0; i < points.size(); i++){
            const int x = points[i].x;
            const int y = points[i].y;
            img.at<float>(x,y) += ajd_mean*influence[i];
        }
        //Should work :)
    }

    struct dijkstra_point{
        dijkstra_point(cv::Point c, std::vector<cv::Point> *v):center(c), visited(v){};

        //Copy
        dijkstra_point(const dijkstra_point& other) 
            : center(other.center), visited(other.visited) {}

        // Assignment operator
        dijkstra_point& operator=(const dijkstra_point& other) {
            if (this != &other) {
                center = other.center;
                visited = other.visited;
            }
            return *this;
        }

        cv::Point center;
        std::vector<cv::Point> *visited;
    };


    //TODO:
    //Decide if algorithm will go 8 ways or 4 ways;
    static void dijkstra_mean_alter(cv::Mat &img_o,cv::Point start_point, int brightnes,int radius = 50, int alter_radius = 100, int threshold=20){
        Entites::Queue<Transformations::dijkstra_point> queue;
        cv::Mat img;
        img_o.convertTo(img, CV_32F);
        cv::waitKey(0);
        std::vector<cv::Point> visited_right;
        std::vector<cv::Point> visited_left;
        std::vector<cv::Point> visited_top;
        std::vector<cv::Point> visited_bottom;
        
        //variables to get from queue
        std::vector<cv::Point> *visited;
        cv::Point center = start_point;

        //Initial queue:
        queue.append(dijkstra_point(cv::Point(center.x + radius, center.y),&visited_right));
        queue.append(dijkstra_point(cv::Point(center.x, center.y + radius),&visited_top));
        queue.append(dijkstra_point(cv::Point(center.x - radius, center.y),&visited_left));
        queue.append(dijkstra_point(cv::Point(center.x, center.y - radius),&visited_bottom));

        alter_mean_of_circle(img,center,brightnes,alter_radius);

        while(!queue.empty()){
            const dijkstra_point p = queue.pop();
            visited = p.visited;
            center = p.center;
            if(center.x < 0 || center.x > img.rows || center.y < 0 || center.y > img.cols){
                //If center is out of bonds, continue
                //TODO:
                //change continue to be on the bonds (on the border)
                continue;
            }
            if(std::find(visited->begin(), visited->end(), center) != visited->end()){
                continue;
            }
            alter_mean_of_circle(img,center,brightnes,alter_radius,threshold);
            visited->push_back(cv::Point(center));
            queue.append(dijkstra_point(cv::Point(center.x + radius, center.y),visited));
            queue.append(dijkstra_point(cv::Point(center.x, center.y + radius),visited));
            queue.append(dijkstra_point(cv::Point(center.x - radius, center.y),visited));
            queue.append(dijkstra_point(cv::Point(center.x, center.y - radius),visited));
        }
        img.convertTo(img_o,CV_8U);
    }
};


