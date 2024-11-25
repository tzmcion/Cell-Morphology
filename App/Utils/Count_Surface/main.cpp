#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"

/**
 *  Component Created by Tymoteusz Apriasz
 *  ©Avant 2024/2025
 *  Process: Counting of total cell surface on image
 *  Args:  
 *  Total of 3 obligatory arguments
 *  @param option_files path to option files, can be "def" to use default options
 *  @param image_list list of paths of images
 *  @param output_folder path to output folder
 *  TEMPLATE: ./main.out def [/home/im1.JPG,/home/im2.JPG] /home/work/output_surface
 * */

int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 4){
        throw std::invalid_argument("Number of argumnets is invalid, required is 3");
    }
    const char* OPTIONS_PATH = argv[1];
    const char* PATHS = argv[2];
    const char* OUT_FOLDER = argv[3];
    const std::string INP_DATA = Entites::Convert::text_file_to_string("../Count_Surface/README.md");
    std::cout << Colors::BG_BRIGHT_GREEN << INP_DATA << Colors::RESET;
    std::cout << "-------------------------------------------------- \n";
    std::cout << "Reading input files, note that this process will end in infinite loop if provided array does not end with \"]\"! \n";
    std::vector<std::string> images;
    Entites::Convert::c_char_to_string(images,PATHS);
    for(size_t x = 0; x < images.size(); x++){
        std::cout << "Image_" << x << ' ' << Colors::MAGENTA << images[x] << Colors::RESET << std::endl;
    }
    std::cout << "Detected " << images.size() << " files to convert in total \n";
    std::cout << "Output folder is: " << Colors::YELLOW <<  OUT_FOLDER << Colors::RESET << std::endl;
    Entites::FILES::folder_create(OUT_FOLDER);
    std::cout << Colors::RED << "Reading options from provided file" << Colors::RESET << std::endl;
    const std::string out_file = std::string(OUT_FOLDER) + "/DATA.csv";
    Entites::FILES::clear_file(out_file.c_str());
    //TODO
    //NOW DO FOR DEFAULT VALUES
    for(size_t x = 0; x < images.size(); x++){
        const std::string PATH = images[x];
        std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
        std::cout << std::flush;
        cv::Mat image;
        image = cv::imread(PATH, cv::IMREAD_GRAYSCALE);
        //
        //ALGORYTHM
        //
        cv::Mat background_mask, foreground_mask;
        Watershed::background_mask(image,background_mask);
        Watershed::foreground_regions(image,foreground_mask,background_mask);
        foreground_mask *=2;    //Foreground mask regions is 2
        cv::Mat markers = foreground_mask.clone();
        markers.setTo(1, background_mask == 0); // Ensure background is labeled as 1
        markers.setTo(0, (background_mask != 0) & (foreground_mask == 0)); // Unknown regions are 0
        cv::Mat image_blured,image_watershed, result,watershed_markers;
        Transformations::opening(image_blured,image,'O',3);
        Watershed::clache(image_blured,image_blured,2.0);
        cv::medianBlur(image_blured,image_blured,3);
        cv::cvtColor(image_blured, image_watershed, cv::COLOR_GRAY2BGR);
        markers.convertTo(watershed_markers,CV_32S);
        cv::watershed(image_watershed,watershed_markers);
        cv::cvtColor(image,result,cv::COLOR_GRAY2RGB);
        int count_green = 0;
        for (int i = 0; i < markers.rows; i++) {
            for (int j = 0; j < markers.cols; j++) {
                if(foreground_mask.at<uchar>(i,j) != 0){
                    //cv::circle(result, cv::Point(j, i), 1, cv::Scalar(255,0,255), -1);
                }
                int markerValue = watershed_markers.at<int>(i, j);
                if (markerValue == -1) {
                    result.at<Vec3b>(i, j) = Vec3b(0, 255, 0); // Red for watershed boundaries
                }
                else if (markerValue == 1) {
                    result.at<Vec3b>(i, j) = Vec3b(2, 0, 255); // Blue for background
                }
                else if (markerValue >= 2) {
                    count_green += 1;
                    result.at<Vec3b>(i, j) = Vec3b(0,255,0);
                }
            }
        }
        Mat imageBGRA;
        cvtColor(image, imageBGRA, COLOR_BGR2RGB);

        // Blend the watershed result with the original image using 20% opacity for the result
        Mat blended;
        addWeighted(imageBGRA, 0.9, result, 0.1, 0, blended); // 80% original + 20% overlay

        // Display the result
        // imshow("Original Image", image);
        // imshow("Watershed Segmentation with Opacity Overlay", blended);
        // waitKey(1);

        std::string Ratio = std::to_string(count_green*100 / double(image.rows*image.cols));
        Entites::FILES::write_to_file(out_file.c_str(),PATH, ';',false);
        Entites::FILES::write_to_file(out_file.c_str(),Ratio);
        //
        //END OF ALGORYTHM
        //
        std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
        std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,blended);
        std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.txt");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
}