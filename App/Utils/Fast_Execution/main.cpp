/**
 * This file provides a way to execute all the preinstalled programs in the same run, avoiding extensive calculations
 * Executed programs/algorithms/utils are:
 * - Count_Cells
 * - Count_Surface
 * - Track_Movement
 * 
 * The intended way of running this file is providing it with OPTIMIZED options, preferabble optimized per cells set (like B, C, D)
 * The output of this are 4 folders with data for each of the algorithms.
 * 
 * As the output, the file creates for each provided image a folder, which has:
 * - count_cells .csv and image
 * - count_surface .csv and image
 * - background and foreground masks - for future Full_Segmentation 
 * - Tracker .csv with each patch and .csv with every 4 patches.
 * 
 * EXPECTED INPUT ARGUMENTS:
 * 1. .aloption file with algorithm options, optimized for specified folder
 * 2. Paths to files (preferabble one study, like B1 with 17 images etc.)
 * 3. Out folder in which the new folders with results will be created
 */

#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Threading/Threads.h"
#include "../../Components/Structures/AlgorithmOptions.h"
#include "../../Components/Tracking/Tracking.h"
#include "../../Components/Tracking/Distribution.h"
#include <mutex>

std::mutex g_i_mutex; 
 
int main(int argc, char**argv){
   cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
   if(argc != 4){
   throw std::invalid_argument("Number of argumnets is incorrect, required is 3");
   }
   const char* OPTIONS_PATH = std::string(argv[1]) == "def" ? "../algorithm_default.option" : argv[1];
   const char* PATHS = argv[2];
   const char* OUT_FOLDER = argv[3];
   AlgorithmOptions options(OPTIONS_PATH);
   const std::string COUNT_PATH = std::string(OUT_FOLDER) + "/COUNT";
   const std::string SURFACE_PATH = std::string(OUT_FOLDER) + "/SURFACE";
   const std::string TRACKING_PATH = std::string(OUT_FOLDER) + "/TRACKING";
   const std::string MASKS_PATH = std::string(OUT_FOLDER) + "/MASKS";
   Entites::FILES::folder_create(OUT_FOLDER);
   Entites::FILES::folder_create(COUNT_PATH.c_str());
   Entites::FILES::folder_create(SURFACE_PATH.c_str());
   Entites::FILES::folder_create(TRACKING_PATH.c_str());
   Entites::FILES::folder_create(MASKS_PATH.c_str());
   std::vector<std::string> images;
   std::vector<std::string> result_paths;
   Entites::Convert::c_char_to_string(images,PATHS);
   size_t cores = std::thread::hardware_concurrency();
   if(cores == 0){
       cores = 1;
   }
   Threading threads(cores);
   for(size_t x = 0; x < images.size(); x++){
      threads.enqueueTask([&images, x, &options, OUT_FOLDER,COUNT_PATH,SURFACE_PATH,TRACKING_PATH,MASKS_PATH,OPTIONS_PATH](){
         const std::string CURR_IMG = images[x];
         const std::string FILENAME = Entites::FILES::extract_filename(CURR_IMG.c_str());
         std::cout << "[]" << " Under operation::: " << Colors::MAGENTA << CURR_IMG << Colors::RESET << std::endl;
         cv::Mat image = cv::imread(CURR_IMG, cv::IMREAD_GRAYSCALE);
         cv::Mat background_mask, foreground_regions, foreground_mask, labels, blended, wat_out;
         //Read from options file (which is a pain in thi ass)
         //This will be very long...
         Watershed::background_mask(image,background_mask,options.get_int_var(0,"BACKGROUND_MASK"),options.get_int_var(1,"BACKGROUND_MASK"),options.get_db_var(2,"BACKGROUND_MASK"),options.get_int_var(3,"BACKGROUND_MASK"),options.get_int_var(4,"BACKGROUND_MASK"),options.get_int_var(5,"BACKGROUND_MASK"),options.get_db_var(6,"BACKGROUND_MASK"),options.get_int_var(7,"BACKGROUND_MASK"));
         // //This will be even longer
         Watershed::foreground_regions(image,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_REGIONS"),options.get_int_var(1,"FOREGROUND_REGIONS"),options.get_db_var(2,"FOREGROUND_REGIONS"),options.get_int_var(3,"FOREGROUND_REGIONS"),options.get_db_var(4,"FOREGROUND_REGIONS"),options.get_int_var(5,"FOREGROUND_REGIONS"));
         // //And thiss will be the longest
         Watershed::foreground_mask(image,foreground_mask,foreground_regions,background_mask,options.get_int_var(0,"FOREGROUND_MASK"), options.get_int_var(1,"FOREGROUND_MASK"), options.get_int_var(2,"FOREGROUND_MASK"), options.get_int_var(3,"FOREGROUND_MASK"), options.get_db_var(4,"FOREGROUND_MASK"), options.get_db_var(5,"FOREGROUND_MASK"),options.get_db_var(6,"FOREGROUND_MASK"), options.get_db_var(7,"FOREGROUND_MASK"), options.get_int_var(8,"FOREGROUND_MASK"), options.get_int_var(9,"FOREGROUND_MASK"), options.get_int_var(10,"FOREGROUND_MASK"), options.get_db_var(11,"FOREGROUND_MASK"), options.get_int_var(12,"FOREGROUND_MASK"));
         // //Applied nhere WOW
         Watershed::watershed_with_masks(image,wat_out,background_mask,foreground_mask,options.get_int_var(0,"WATERSHED"),options.get_int_var(1,"WATERSHED"),options.get_int_var(2,"WATERSHED"),options.get_db_var(3,"WATERSHED"),options.get_int_var(4,"WATERSHED"));
         cv::Mat foreground_mask_save;
         cv::compare(foreground_mask, 0, foreground_mask_save, cv::CMP_NE);
         cv::imwrite(MASKS_PATH + "/FOREGROUND_" + FILENAME + "bmp", foreground_mask_save);
         cv::imwrite(MASKS_PATH + "/BACKGROUND_" + FILENAME + "bmp", background_mask);
         //FIRST, CELL COUNT
         cv::Mat wat_for_connected = wat_out > 1;
         cv::Mat areas, centroids;
         const int cells = cv::connectedComponentsWithStats(wat_for_connected,labels, areas, centroids);
         cv::cvtColor(image,blended,COLOR_GRAY2BGR);
         for(int x = 0; x < cells; x++){
            cv::Point center(cvRound(centroids.at<double>(x,0)),cvRound(centroids.at<double>(x,1)));
            cv::circle(blended, center, 1, cv::Scalar(0,255,0), -1);
         }
         const std::string OUT_PATH_COUNT = COUNT_PATH + "_data.csv";
         std::string Ratio = std::to_string(cells);
         Entites::FILES::write_to_file(OUT_PATH_COUNT.c_str(),CURR_IMG, ';',false);
         Entites::FILES::write_to_file(OUT_PATH_COUNT.c_str(),Ratio);
         cv::imwrite(COUNT_PATH + "/" + FILENAME + "jpg",blended);
         //
         //SURFACE_COUNT !!!DO ZMIANY, TRZEBA NA TO OSOBNY WATERSHED Z FOREGORUND REGIONS
         //
         cv::Mat watershed_for_surface;
         Watershed::watershed_with_masks(image,watershed_for_surface,background_mask,foreground_regions, options.get_int_var(0,"WATERSHED"),options.get_int_var(1,"WATERSHED"),options.get_int_var(2,"WATERSHED"),options.get_db_var(3,"WATERSHED"),options.get_int_var(4,"WATERSHED"));
         cv::Mat result;
         cv::cvtColor(image,result,cv::COLOR_GRAY2RGB);
         int count_green = 0;
         for (int i = 0; i < watershed_for_surface.rows; i++) {
             for (int j = 0; j < watershed_for_surface.cols; j++) {
                 if(foreground_mask.at<uchar>(i,j) != 0){
                     cv::circle(result, cv::Point(j, i), 1, cv::Scalar(255,0,255), -1);
                 }
                 int markerValue = watershed_for_surface.at<int>(i, j);
                 if (markerValue == -1) {
                     count_green += 1;
                     result.at<Vec3b>(i, j) = Vec3b(0, 255, 0); // Count as background, Green
                 }
                 else if (markerValue == 1) {
                     result.at<Vec3b>(i, j) = Vec3b(20, 20, 255); // Red for background
                 }
                 else if (markerValue >= 2) {
                     count_green += 1;
                     result.at<Vec3b>(i, j) = Vec3b(0,255,0);   //Green
                 }
             }
         }
         Mat imageBGRA;
         cvtColor(image, imageBGRA, COLOR_BGR2RGB);
         addWeighted(imageBGRA, 0.6, result, 0.4, 0, blended); // 80% original + 20% overlay
         const std::string OUT_PATH_SURF = SURFACE_PATH + "_data.csv";
         Ratio = std::to_string(count_green*100 / double(image.rows*image.cols));
         Entites::FILES::write_to_file(OUT_PATH_SURF.c_str(),CURR_IMG, ';',false);
         Entites::FILES::write_to_file(OUT_PATH_SURF.c_str(),Ratio);
         cv::imwrite(SURFACE_PATH + "/" + FILENAME + "jpg",blended);
         //
         //TRACKING
         //
         //
         const std::lock_guard<std::mutex> lock(g_i_mutex);
         std::vector<int> occurances;
         double min,max,avg;
         const std::string OUT_PATH_TRACKING = TRACKING_PATH + "/" + FILENAME + "csv";
         std::cout << FILENAME << std::endl;
         std::cout << OUT_PATH_TRACKING << std::endl;
         Tracking::radius_by_centers(wat_for_connected,OPTIONS_PATH,occurances,min,max,avg);
         Tracking::distribution_to_csv(OUT_PATH_TRACKING.c_str(),occurances,min,max);
      });
   }
   threads.awaitJoin();
   //Here waiting to all threads finish the job, and then do the sorting algorithms.
   const std::string OUT_PATH_SURF = SURFACE_PATH + "_data.csv";    //sort this one
   const std::string OUT_PATH_COUNT = COUNT_PATH + "_data.csv";     //sort this one
   //TRACKING_PATH = folder from which the csvs needs to be read.
   Entites::FILES::CSV_sort(OUT_PATH_SURF.c_str());
   Entites::FILES::CSV_sort(OUT_PATH_COUNT.c_str());
   //Up to this point, it works
   //Read all files in a directory
   std::vector<std::string> paths_to_merge;
   std::string path = TRACKING_PATH;
   int count = 0;
   for (const auto & entry : std::filesystem::directory_iterator(path)){
        paths_to_merge.push_back(entry.path());
   }
   std::sort(paths_to_merge.begin(), paths_to_merge.end());
   std::vector<std::string> to_merge;
   //Next code :)
   for(size_t i = 0; i < paths_to_merge.size(); i++){
   to_merge.push_back(paths_to_merge[i]);
   if(i % 4 == 0){
        const std::string save_path = TRACKING_PATH + "/merged_" + std::to_string(count) + ".csv"; 
        std::cout << save_path << std::endl;
        Merger::CSV_merge(to_merge,save_path.c_str());
        to_merge.clear();
        count+=1;
        }
   }
   return 0;
}