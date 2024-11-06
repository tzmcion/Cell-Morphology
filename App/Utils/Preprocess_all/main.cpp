#include "../../Components/Transformations/Transformations.h"
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
 * */

int main(int argc, char** argv){
    ReadOptions reader("./defaul_settings.option");
    std::cout << reader.get_data_size() << std::endl;
    std::cout << "Total nr of operations: " << reader.get_total_operations() << std::endl;
    do{
        std::cout << "Current operation: " << reader.get_folder_name() << std::endl;
        std::cout << "Current arguments: " << reader.get_arguments() << std::endl;
    }
    while(reader.next_run());
    return 0;
}