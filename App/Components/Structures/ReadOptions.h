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
     * Function returns number of total operations which will run
     * */
    int get_total_operations(size_t idx = 0){
        if(idx >= data.size())return 0;
        return data[idx]->iterations + get_total_operations(idx+1);
    }

    bool next_run();

    private:

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
};