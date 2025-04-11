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
         * The value of distribution can be calculated using min_val_calc, max_val_calc, and vector size.
         * @returns averages - vector of occurances of data in certain ranges (distribution)
         * @returns min_val_calc - value of first cluster/range in vector of occurances
         * @returns max_val_calc - value of last cluster/range in vector of occurances. Can be then calculated and the chart can be drawn
         * @param patch - watershedMask, image which underwent watershed segmentation, or foreground mask
         * @param options_path - path to file with options
         * @param averages vector of integers, where stored will be the distribution.
         * @param min_val_calc value of first element in vector averages, being the first cluster of data in distribution
         * @param max_val_calc value of last element in vector averages, stating maximum distribution value
         */
        static void radius_by_centers(cv::Mat &patch, const char* options_path, std::vector<int> &averages, double &min_val_calc, double &max_val_calc);

        /**
         * Function prints the distribution of vector with data in console
         * The chart is calculated using vector size, left and right values, being ch_min_val and ch_max_val
         * @param distribution - vector with data
         * @param chart_height - height of the chart, in cout lines
         * @param ch_min_val min value of distribution
         * @param ch_max_val maximum value in distribution
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
        static double _get_closest_point(cv::Point point, cv::Mat& matrix, int object_id, double min_radius, double max_radius);

        /**
         * Function calculates which index should be the value in distribution
         * By default function respects in return size_t min=0, and max=9 
         */
        static size_t _vector_index_by_value(double min_distr_val, double max_distr_val, double val, size_t steps){
            const double max_val = max_distr_val-min_distr_val;
            const double val_to_check = val - min_distr_val;
            return static_cast<size_t>(((val_to_check/max_val) * steps));
        }
};

