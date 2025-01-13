#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Structures/Colors.h"

/**
 *  Component Created by Tymoteusz Apriasz
 *  ©Avant 2024/2025
 *  Process Normalizes the brightness on set of the images, by using their overall mean
 *  Args:  [path1,path2,...,path(n)], output_folder
 *  Example of input: ./main.out [./d1.jpg,./d2.jpg,./d3.jpg,./d4.jpg] ./temp_img
 * */

//BEZ SENSU
//ZROB PO PROSTU KAZDY PIXEL A NIE TE KOLKA
//ONE TUTAJ SA NA NIC!!!!

int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 3){
        throw std::invalid_argument( "Number of Arguments is too little, required is 5");
    }
    const char* PATHS = argv[1];
    const char* OUT_FOLDER = argv[2];
    std::string INP_DATA = Entites::Convert::text_file_to_string("../Norm_br_global/README.md");
    std::cout << Colors::CYAN << INP_DATA << Colors::RESET;
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
    std::cout << "Variables: \n";
    std::cout << "No variables for this process" <<std::endl;
    std::cout << "Starting the processment of data: \n";
    std::cout << "[...] \n";
    std::cout << "Starting the process of calculating the best mean \n";
    double mean_br_of_images = 0;
    for(size_t x = 0; x < images.size(); x++){
        cv::Mat img = cv::imread(images[x], cv::IMREAD_GRAYSCALE);
        mean_br_of_images += Transformations::image_brightnes(img);
    }
    mean_br_of_images /= images.size();
    std::cout << "Mean brightness calculated as: " << mean_br_of_images << " normalizing brightness on images... \n \n";
    for(size_t x = 0; x < images.size(); x++){
        const std::string PATH = images[x];
        std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
        std::cout << std::flush;
        cv::Mat img;
        img = cv::imread(PATH,cv::IMREAD_GRAYSCALE);
        Transformations::alter_brightnes(img,mean_br_of_images);
        std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
        std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,img);
        std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.umsg");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
}