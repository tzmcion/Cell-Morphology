/**
 * Compilation file for "Structures/ReadOptions.h"
 * */

#include "./PrepAllOptions.h"

ReadOptions::ReadOptions(const char* options_path){
    if(strcmp(options_path, "NULL") == 0)return;
    original = false;   //set original to false by default
    std::vector<std::string> lines;
    std::ifstream file(options_path);   //load the file
    
    //Read the file and copy lines to a vector
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open the file: " << options_path << std::endl;
    }

    //Read the vector and process the lines
    run_file *raw = nullptr;
    for(size_t x = 0; x < lines.size(); x++){
        //clear the line to get raw input
        std::vector<std::string> line = this->split_string(this->clear_line(lines[x]),' ');
        //Get the line task
        if(line.empty())continue;
        std::string task = line[0];
        //Check what type of line
        if(task == "!ORG"){
            original = true;
            continue;
        }
        if(task == "ID"){
            if(raw != nullptr) {
                if(!raw->is_ready()){
                    throw std::invalid_argument("Option file is borken, program corrupted");
                }
                data.push_back(raw);
            }
            raw = new run_file();
            raw->folder_name = line[1];
        }
        else if(task == "arguments"){
            std::string whole_line = lines[x];
            std::string argument = "";
            bool add_c = false;
            for(size_t y = 0; y < whole_line.length(); y++){
                char c = whole_line[y];
                if(c == '['){
                    add_c = true;
                    continue;
                }
                if(c == ']'){
                    add_c=false;
                    raw->arguments.push_back(argument);
                    argument = "";
                }
                if(add_c){
                    argument += c;
                }
            }
        }
        else if(task == "iterations"){
            raw->iterations = std::atoi(line[1].c_str());
        }else{
            std::cout << Colors::RED << "[ERROR] " << Colors::YELLOW << "option name not recognized: " << Colors::RESET << task << Colors::BLUE << " ommiting the line...";
        }
    }
    //Add To data ready component
    data.push_back(raw);
}

//Free the memory
ReadOptions::~ReadOptions(){
    for(size_t x = 0; x < data.size(); x++){
        delete data[x];
    }
}

std::string ReadOptions::get_arguments(){
    return data[index]->arguments[subindex];
}

std::string ReadOptions::get_folder_name(){
    return data[index]->folder_name;
}

bool ReadOptions::next_run(){
    //Get number of iterations in current
    size_t current_iters = static_cast<size_t>(data[index]->iterations);
    if(subindex < (current_iters-1)){
        subindex+=1;
        return true;
    }
    if(index + 1 < data.size()){
        index+=1;
        subindex=0;
        return true;
    }
    return false;
}

//PRIV

//Clear line from comments writen after '#' symbol
std::string ReadOptions::clear_line(std::string l){
    std::string line = l,segment;
    std::replace( line.begin(), line.end(), ':', ' ');
    //Now split the line by "#" to exclude comments
    std::stringstream s(line);
    while(std::getline(s, segment, '#'))
    {
        return segment;
    }
    return line;
}

//Split the string into segments
std::vector<std::string> ReadOptions::split_string(std::string s, char spliter){
    std::stringstream str(s);
    std::vector<std::string> out;
    std::string segment;
    while(std::getline(str, segment, spliter))
    {
        if(segment == " " || segment == "" || segment == "  " || segment == "   ") continue;
        out.push_back(segment);
    }
    return out;
}
//
void ReadOptions::error_message(const char* path, std::string line,int line_index){
    //First, handle when path to file is not defined
    std::string file_path = (path == NULL || path == nullptr) ? std::string(std::filesystem::canonical("/proc/self/exe")) : std::string(path);
    std::string err_message = std::string(Colors::RED) + "\n[ERROR]" + std::string(Colors::RESET) + " Options for algorithm " + file_path;
    if(line != ""){
        std::string line_nr = line_index == -1 ? "" : std::to_string(line_index);
        err_message += std::string(Colors::CYAN) + "\n[LINE] " + std::string(Colors::RESET)+ line_nr + " : \"" + line + "\"";
    }
    throw std::invalid_argument(err_message.c_str());
}