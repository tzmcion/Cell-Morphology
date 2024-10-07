#pragma once

#include "../Includes/includes.h"
#include "../Structures/Structures.h"

class Transformations{
    public:
    /*Function returns average brightnes of the image*/
    static double image_brightnes(cv::Mat img);

    /*Function alters the image brightnes within the file (opens and saves it)*/
    static void alter_brightnes(std::string path, double brightnes);

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
     *  Function removes dots using connected components, can be removed by size and
     *  @param threshold_black states how black mean of object can be
     *  @param threshold_size states min size of object which is accepted
     * */
    static void dots_remove(cv::Mat &img, int threshold_black=100, int threshold_size=50);

    /**
     *  Function normalizes brightnes of the image
     *  @param img cv.Mat imag3
     *  @param max_radius Maximum radius of the mean brightness of the area
     * */
    static void norm_brightnes(cv::Mat &img, int max_radius=50, int alter_radius=200);


    private:

    static void is_image(cv::Mat &img, std::string path="certain_path"){
        if(img.empty()){
            const std::string msg = "Could not open the image on path: " + path;
            throw std::invalid_argument(msg);
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
    static void alter_mean_of_circle(cv::Mat &img, cv::Point center, int goal_brightness, int radius=50){
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
        for(int x = x_min; x < x_max; x++){
            for(int y = y_min; y < y_max; y++){
                const int distance = std::floor(std::sqrt(std::pow(x - center.x,2) + std::pow(y - center.y,2)));
                if(distance <= radius){
                    sum += img.at<cv::uint8_t>(x,y);
                    count += 1;
                    cv::Point ap_point(x,y);
                    points.push_back(ap_point);
                }
            }
        }
        double ajd_mean = goal_brightness - sum/count;
        //Now, the loop has to be done again to alter the points depending on brightnes
        //Let's also add statament that alter cannot be bigger than 5
        if(abs(ajd_mean) > 4){ajd_mean = 4 * (ajd_mean > 0 ? 1 : -1);}
        for(int i = 0; i < points.size(); i++){
            const int x = points[i].x;
            const int y = points[i].y;
            img.at<cv::uint8_t>(x,y) = cv::saturate_cast<cv::uint8_t>(img.at<cv::uint8_t>(x,y) + ajd_mean);
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
    static void dijkstra_mean_alter(cv::Mat &img,cv::Point start_point, int brightnes,int radius = 50, int alter_radius = 100){
        Entites::Queue<Transformations::dijkstra_point> queue;
        //vectors collecting already visited points
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
                //Here check if this point was already visited by this instance
                continue;
            }
            alter_mean_of_circle(img,center,brightnes,alter_radius);
            visited->push_back(cv::Point(center));
            queue.append(dijkstra_point(cv::Point(center.x + radius, center.y),visited));
            queue.append(dijkstra_point(cv::Point(center.x, center.y + radius),visited));
            queue.append(dijkstra_point(cv::Point(center.x - radius, center.y),visited));
            queue.append(dijkstra_point(cv::Point(center.x, center.y - radius),visited));
        }
    }
};


