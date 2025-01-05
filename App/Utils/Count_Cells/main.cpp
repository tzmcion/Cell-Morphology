#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Threading/Threads.h"

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
    const std::string INP_DATA = Entites::Convert::text_file_to_string("../Count_Cells/README.md");
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
    const std::string out_file = std::string(OUT_FOLDER) + "/DATA" + "_COUNT" + ".csv";
    Entites::FILES::clear_file(out_file.c_str());
    //TODO
    //NOW DO FOR DEFAULT VALUES
    size_t cores = std::thread::hardware_concurrency();
    if(cores == 0){
        cores = 3;
    }
    Threading threads(cores);
    for(size_t x = 0; x < images.size(); x++){
        threads.enqueueTask([x,OUT_FOLDER,out_file,images](){
            const std::string PATH = images[x];
            std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
            std::cout << std::flush;
            cv::Mat image;
            image = cv::imread(PATH, cv::IMREAD_GRAYSCALE);
            //
            //ALGORYTHM
            //
            cv::Mat background_mask, foreground_regions, foreground_mask, labels, blended;
            Watershed::background_mask(image,background_mask);
            Watershed::foreground_regions(image,foreground_regions,background_mask);
            Watershed::foreground_mask(image,foreground_mask,foreground_regions,background_mask);
            const int cells = cv::connectedComponents(foreground_mask,labels);
            cv::cvtColor(image,blended,COLOR_GRAY2BGR);
            for(int x = 0; x < image.rows; x++){
                for(int y = 0; y < image.cols; y++){
                    if(foreground_mask.at<uchar>(x,y) != 0){
                        cv::circle(blended, cv::Point(y, x), 1, cv::Scalar(0,255,0), -1);
                    }
                }
            }
            std::string Ratio = std::to_string(cells);
            Entites::FILES::write_to_file(out_file.c_str(),PATH, ';',false);
            Entites::FILES::write_to_file(out_file.c_str(),Ratio);
            //
            //END OF ALGORYTHM
            //
            std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
            std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,blended);
            std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
        });
    }
    return 0;
}