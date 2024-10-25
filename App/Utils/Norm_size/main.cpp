#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Colors.h"

/**
 *  Component Created by Tymoteusz Apriasz \n
 *  ©Avant 2024/2025 \n
 *  Process: Croping the image to square and resizing to provided size \n
 *  Args: 
 *  Total of 3 params
 *  @param d_size desired size of image
 *  @param inp_dir input_images [../i2.jpg,../i3.jpg]
 *  @param out_dir output_folder
 *  TEMPLATE: ./main.out 500 [../fold/mic/dada/jpg,../fold/mid/dadb.jpg] C:/users/tymota/out_folder
 * */

int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc < 4){
        throw std::invalid_argument("Error, number of arguments is to little, expected 4");
    }
    const int D_SIZE = std::atoi(argv[1]);
    const char* PATHS = argv[2];
    const char* OUT_FOLDER = argv[3];
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
    std::cout << "Desired Size:: " << D_SIZE << std::endl;
    std::cout << "Starting the processment of data: \n";
    for(size_t x = 0; x < images.size(); x++){
        const std::string PATH = images[x];
        std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
        std::cout << std::flush;
        cv::Mat img;
        img = cv::imread(PATH,cv::IMREAD_GRAYSCALE);
        Transformations::square_and_resize(img,D_SIZE);
        std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
        std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,img);
        std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.txt");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
    return 0;
}