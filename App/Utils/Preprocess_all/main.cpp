#include "../../Components/Structures/Structures.h"
#include "../../Components/Structures/ReadOptions.h"

/**
 *  Component Created by Tymoteusz Apriasz
 *  ©Avant 2024/2025
 *  Process: Running all the preprocessing for multiple images
 *  Input: Options & List of Paths to the images
 *  Output: processed images in specified folder
 *  Description of process: 
 *      The process usess arguments specified in the options file to run each of processes
 *      Then the algorithms are run in the images, saving them during the time in temp folder
 *  Input: options link_to_file_with_paths out_folder
 *  Input:
 *  use "def" to use default options as options
 *  Example: ./main.out def ./paths.txt ./out_folder
 *  paths.txt must be in a form of [../l.jpg,../l2.jpg]...
 * */

int main(int argc, char** argv){
    if(argc != 4){
        throw std::invalid_argument("    [ERROR]: number of arguments is invalid");
    }
    const char* OPTIONS = std::string(argv[1]) == "def" ? "./default_settings.option" : argv[1];
    std::string PATHS = Entites::Convert::text_file_to_string(argv[2]);
    const char *OUT_FOLDER = argv[3];
    const std::string README = Entites::Convert::text_file_to_string("./README.md"); 
    std::cout << Colors::GREEN << README << Colors::RESET;
    std::cout << "----------------------------------------- \n";
        std::cout << "Reading input files, note that this process will end in infinite loop if provided array does not end with \"]\"! \n";
    std::vector<std::string> images;
    Entites::Convert::c_char_to_string(images,PATHS.c_str());
    for(size_t x = 0; x < images.size(); x++){
        std::cout << "Image_" << x << ' ' << Colors::MAGENTA << images[x] << Colors::RESET << std::endl;
    }
    std::cout << "Detected " << images.size() << " files to convert in total \n";
    std::cout << "Output folder is: " << Colors::YELLOW <<  OUT_FOLDER << Colors::RESET << std::endl;
    Entites::FILES::folder_create(OUT_FOLDER);
    //Must also read options
    if(std::string(argv[1]) == "def"){
        std::cout << Colors::BRIGHT_RED << "    [...] WARNING" << Colors::RESET << " No path for options provided, Automatically assigned file options as \"./default_settings.option\" \n";
    }
    std::cout << "OPTIONS: " << OPTIONS << std::endl;
    std::cout << "----------------------------------------- \n";
    std::cout << "Starting the processment of data: \n";
    std::cout << "Reading options file: \n";
    ReadOptions reader(OPTIONS);
    std::cout << Colors::BRIGHT_BLUE << "    [...] INFO" << Colors::RESET << " Total number of programs to run: " << reader.get_data_size() << std::endl;
    std::cout << Colors::BRIGHT_BLUE << "    [...] INFO" << Colors::RESET << " Total nr of iterations: " << reader.get_total_operations() << std::endl;
    std::cout << "Copying files to temp folder "<< OUT_FOLDER << std::endl;
    for(size_t x = 0; x < images.size(); x++){
        cv::Mat img = cv::imread(images[x]);
        std::cout << Colors::YELLOW << " [...] COPY: " << Colors::RESET << images[x] << " to: " << OUT_FOLDER << std::endl;
        images[x] = Entites::FILES::save_to_folder(images[x],OUT_FOLDER,img);
    }
    std::cout << "Starting iterations: " << std::endl;
    system("echo \"OUTPUT\" > ./program_info.ans");
    system("gnome-terminal --geometry=120x30 -- bash -c 'tail -f ./program_info.ans; exec bash'");
    do{
        std::cout << Colors::MAGENTA << "  [...] PROCESSMENT: " << Colors::RESET << reader.get_folder_name() << std::endl;
        std::cout << " [...] Current arguments: " << reader.get_arguments() << std::endl;
        std::string _input = "";
        _input += "../" + reader.get_folder_name() + "/main.out";
        if(reader.get_arguments() != ""){_input += " ";}
        _input += reader.get_arguments() + " [";
        for(size_t x = 0; x < images.size(); x++){
            if(x != 0)_input += ',';
            _input += images[x];
        }
        _input += "] " + std::string(OUT_FOLDER);
        if(reader.original){
            _input += '/' + reader.get_folder_name();
        }
        _input += " >> ./program_info.ans";
        std::cout << Colors::YELLOW << "[COMMAND:] " << Colors::RESET << _input << std::endl;
        system(_input.c_str());
        std::cout << "Command sucessfully finished! \n";
    }
    while(reader.next_run());
    return 0;
}