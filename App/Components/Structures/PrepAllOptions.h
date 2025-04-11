/**
 * This file is used by "Preprocess_all" Util, and is inheritated by AlgorithmOptions
 * It defines how .option file is being read, and provides error messages in reading
 * It stores the operations in vector which is then iterated by next_run()
 * AlgorithmOptions uses it's method of clearing comments from text file
 * */

#pragma once
#include "../Includes/includes.h"
#include "../Structures/Colors.h"
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <sstream>

class ReadOptions{
    public:
    /**
     * Class to read and iterate through .options file
     * options file is structured specifically for this program
     * @param options_path path to folder with options 
     * */
    ReadOptions(const char* options_path);
    ~ReadOptions();

    std::string get_arguments();
    std::string get_folder_name();

    /**
     * Get how many different folders were detected in settings file
     * */
    int get_data_size(){
        return data.size();
    }

    /**
     * Function returns number of total operations which will be executed
     * */
    int get_total_operations(size_t idx = 0){
        if(idx >= data.size())return 0;
        return data[idx]->iterations + get_total_operations(idx+1);
    }

    /**
     * Function fires next iteration of queue (in shape of vector),
     * returns true if next operation exists, and false if next operation does not exist
     * */
    bool next_run();

    /**
     * Variable used if needed not to alter the files between operations.
     * By default, the "Preprocess_all" Util for which this program was made
     * copies file to the specified directory, and alters them consequently with each
     * specified program in options file. If set to false, program creates copies of the images with
     * every iteration, therefore preserving original files for each iteration
     * */
    bool original;

    protected:

    /**
     * Function clears the line from comments and ":"
     * */
    std::string clear_line(std::string l);

    /**
     * Function splits the string
     * */
    std::vector<std::string> split_string(std::string s, char spliter);
    struct run_file{
        run_file(){
            folder_name = "";
            iterations = 0;
        }
        bool is_ready(){
            if(arguments.empty() || folder_name == "" || iterations == 0)return false;
            return true;
        }
        std::vector<std::string> arguments;
        std::string folder_name;
        int iterations;
    };
    size_t index = 0;
    size_t subindex = 0;
    std::vector<run_file*> data;

    /**
     *  Function Throws a Formatted Error for options file.
     *  It is important to do a good error message here, as .option file is the one
     *  with which user should integrate the most
     *  @param path Optional to nullptr. IF no path, type nullptr
     *  @param line Optional to "". IF no line type ""
     *  @param line_index Optional to -1. IF no line_index, type -1
     * */
    void error_message(const char* path = nullptr, std::string line = "", int line_index = -1);
};