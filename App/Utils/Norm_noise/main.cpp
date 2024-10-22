#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Structures/Colors.h"

/**
 *  Component Created by Tymoteusz Apriasz \n
 *  ©Avant 2024/2025 \n
 *  Process Reducing of the noise on the image using connected components \n
 *  Args:  threshold_gray, max_size, [path1,path2,...,path(n)], output_folder \n
 *  Example of input: ./main.out 100 50 [./d1.jpg, ./d2.jpg, ./d3.jpg, ./d4.jpg] ./temp_img \n
 * */

int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 5){
        throw std::invalid_argument( "Number of Arguments is too little, required is 5");
    }
    const int THRESHOLD = std::atoi(argv[1]);
    const int MAX_SIZE = std::atoi(argv[2]);
    const char* PATHS = argv[3];
    const char* OUT_FOLDER = argv[4];
    std::string INP_DATA = Entites::Convert::text_file_to_string("./README.md");
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
    std::cout << "Threshold: " << THRESHOLD << std::endl;
    std::cout << "Size: " << MAX_SIZE << std::endl;
    std::cout << "Starting the processment of data: \n";
    for(size_t x = 0; x < images.size(); x++){
        const std::string PATH = images[x];
        std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
        std::cout << std::flush;
        cv::Mat image;
        image = cv::imread(PATH, cv::IMREAD_GRAYSCALE);
        Transformations::dots_remove(image,THRESHOLD,MAX_SIZE);
        std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
        std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,image);
        std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.txt");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
}