#pragma once

/**
 * Idk if I should track after classification or before, probably after
 * 
 */

#include "../Includes/includes.h"
#include "../Structures/AlgorithmOptions.h"
#include "../Watershed/Watershed.h"
#include "../Structures/Colors.h"

class Tracking{

    public:
        /**
         * For each found center point on the foreground_map matrix, it finds the closest next center point.
         * If it exceeds the set cell radius by 5 times, the search stops and the radius is set as provided max_radius  
         * @param patches - vector of foreground_masks
         * @param options_path - path to file with options
         */
        static void radius_by_centers(std::vector<cv::Mat> &patches, const char* options_path, std::vector<int> &averages, double &min_val_calc, double &max_val_calc);

        /**
         * Function prints the distribution of vector with data in console
         * @param distribution - vector with data
         */
        static void cout_distribution(std::vector<int> &distribution, int chart_height, double ch_min_val, double ch_max_val);
    // private:

        /**
         * Function finds the point being closest to the current point, but not exceeding max range
         * Returns max_range if no point was found, and distance to the point if point exists in provided range of radius
         * @param matrix - image or map with the points
         * @param object_id - id of the object which is a starting point
         * @param min_radius - starting radius of search
         * @param max_radius - maximum radius which limits the search
         */
        static double get_closest_point(cv::Point point, cv::Mat& matrix, int object_id, double min_radius, double max_radius);

        /**
         * Function calculates which index should be the value in distribution
         * By default function respects in return size_t min=0, and max=9 
         */
        static size_t __vector_index_by_value(double min_distr_val, double max_distr_val, double val, size_t steps){
            const double max_val = max_distr_val-min_distr_val;
            const double val_to_check = val - min_distr_val;
            return static_cast<size_t>(((val_to_check/max_val) * steps));
        }

};

