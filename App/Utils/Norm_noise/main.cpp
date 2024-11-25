#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Threading/Threads.h"

/**
 *  Component Created by Tymoteusz Apriasz
 *  ©Avant 2024/2025
 *  Process Reducing of the noise on the image using connected components
 *  Args:  
 *  Total of 9 params
 *  @param threshold Threshold value for detecting the object as removable (ratio -- mean/25 * threshold_black)
 *  @param area_size size of detected object being to big to be counted as noise
 *  @param kernel_size_dilation_initial size of initial dilation, before the painting of gray over the detected object 
 *  @param kernel_size_dilation_second size of second dilation, after the painting of gray over the detected object
 *  @param inpaint_size size of kernel of inpaint, inpaint merges the objects with background
 *  @param inpaint_type type of inpaint algorithm, 1 or 0 | For small 0 is better
 *  @param input_images_list - List of input files
 *  @param output_folder_dir - directory of output file
 *  @param display_changes set to 1 if need of displaying the process
 *  TEMPLATE: Example of input: ./main.out 15 30 3 5 5 0 0 [./d1.jpg,./d2.jpg,./d3.jpg,./d4.jpg] ./temp_img \n 
 *  Optimal: 15 50 1 2 2 0 0 (I think)
 * */
int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 10){
        throw std::invalid_argument( "Number of Arguments is too little, required is 9");
    }
    const int THRESHOLD = std::atoi(argv[1]);
    const int MAX_SIZE = std::atoi(argv[2]);
    const int KERN_SIZE_INIT = std::atoi(argv[3]);
    const int KERN_SIZE_SEC = std::atoi(argv[4]);
    const int INP_SIZE = std::atoi(argv[5]);
    const int INP_TYPE = std::atoi(argv[6]);
    const bool DISPL_CH = std::atoi(argv[7]) == 1 ? true : false;
    const char* PATHS = argv[8];
    const char* OUT_FOLDER = argv[9];
    std::string INP_DATA = Entites::Convert::text_file_to_string("../Norm_noise/README.md");
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
    std::cout << "Kernel size for initial dilation: " << KERN_SIZE_INIT << std::endl;
    std::cout << "Kernel size for second dilation: " << KERN_SIZE_SEC << std::endl;
    std::cout << "Inpaint size: " << INP_SIZE << std::endl;
    std::cout << "Inpaint type: " << INP_TYPE << std::endl;
    std::cout << "Starting the processment of data: \n";
    //IMPLEMENT THREADING
    size_t cores = std::thread::hardware_concurrency();
    if(cores == 0){
        cores = 3;
    }
    Threading threads(cores);
    //IMPLEMENT THREADING
    for(size_t x = 0; x < images.size(); x++){
        threads.enqueueTask([x,images,THRESHOLD,MAX_SIZE, KERN_SIZE_INIT, KERN_SIZE_SEC, INP_SIZE, INP_TYPE, DISPL_CH, OUT_FOLDER](){
            const std::string PATH = images[x];
            std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
            std::cout << std::flush;
            cv::Mat image;
            image = cv::imread(PATH, cv::IMREAD_GRAYSCALE);
            Transformations::dots_remove(image,THRESHOLD,MAX_SIZE, KERN_SIZE_INIT, KERN_SIZE_SEC, INP_SIZE, INP_TYPE, DISPL_CH);
            std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
            std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,image);
            std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
        });
    }
    // std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.txt");
    // std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
}