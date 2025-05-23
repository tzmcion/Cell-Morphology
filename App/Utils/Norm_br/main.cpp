#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Threading/Threads.h"

/**
 *  Component Created by Tymoteusz Apriasz \n
 *  ©Avant 2024/2025 \n
 *  Process Normalizes Brightness in Image using it's mean brightnes \n
 *  Args:  radius, traverse_radius, max_diff_threshold, [path1,path2,...,path(n)], output_folder \n
 *  Example of input: ./main.out 10 15 30 [./d1.jpg,./d2.jpg,./d3.jpg,./d4.jpg] ./temp_img \n
 * */



int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 6){
        throw std::invalid_argument( "Number of Arguments is too little, required is 5");
    }
    const int RADIUS = std::atoi(argv[1]);
    const int TRAVERSE = std::atoi(argv[2]);
    const int MAX_DIFF = std::atoi(argv[3]);
    const char* PATHS = argv[4];
    const char* OUT_FOLDER = argv[5];
    std::string INP_DATA = Entites::Convert::text_file_to_string("../Norm_br/README.md");
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
    std::cout << "Radius: " << TRAVERSE << std::endl;
    std::cout << "Traverse: " << RADIUS << std::endl;
    std::cout << "MAX_DIFF: " << MAX_DIFF << std::endl;
    //MULTITHREAD
    size_t cores = std::thread::hardware_concurrency();
    if(cores == 0){
        cores = 1;
    }
    Threading threads(cores);
    //MULTITHREAD
    std::cout << "Starting the processment of data: \n";
    for(size_t x = 0; x < images.size(); x++){
        // threads.enqueueTask([&images,RADIUS,TRAVERSE,MAX_DIFF,OUT_FOLDER,x](){
            const std::string PATH = images[x];
            std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
            //std::cout << std::flush;
            cv::Mat img;
            img = cv::imread(PATH,cv::IMREAD_GRAYSCALE);
            Transformations::norm_brightnes(img,RADIUS,TRAVERSE,MAX_DIFF);

            std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
            std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,img);
            std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
        // });
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.umsg");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
}