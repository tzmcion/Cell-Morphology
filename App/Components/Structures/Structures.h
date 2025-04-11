/**
 * This file defines structures and usefull entity functions
 */

#pragma once

#include "../Includes/includes.h"
#include <fstream>
#include <sys/stat.h>

namespace Entites{
    //Typical Queue, used in preprocessing algorithm, in Dijsktra point
    template<typename T>
    class Queue{
        public:

        /**
        * add element to the end of queue
        * */
        void append(T var){
            data.push_back(var);
        }

        /**
         * Deletes and returns first element of queue
         * */
        T pop(){
            T d = this->begin();
            data.erase(data.begin());
            return d;
        }

        /**
         * Returns element on the begin of queue not deleting it
         * */
        T begin() const{
            return data.front();
        }

        bool empty() const{
            return data.size() == 0;
        }

        int size() const {return static_cast<int>(data.size());}

        private:
        std::vector<T> data;
    };

    class Convert{
        /**
         * Function converts c_char list in shape of "[ar1, ar2,..., arN]" to a string vector [ar1, ar2, ..., arN]
         * It is used when providing path list to a executable program
         * */
        public: 
        static void c_char_to_string(std::vector<std::string> &vec, const char* list){
            vec.clear();
            if( list[0] != '[' ){
                throw std::invalid_argument("list passed as argument is not in shape of [ar1,ar2] !! DELETE SPACES IF YOU HAVE");
            }
            int i = 1;
            while(1){
                std::string value = "";
                while(list[i] != ','){
                    if(list[i] == ']'){
                        //add to vec and return function
                        vec.push_back(value);
                        return;
                    }
                    value += list[i];
                    i++;
                }
                i++;
                vec.push_back(value);
            }
        }

        /**
         * Function converts text saved in a file to a string.
         * Used in reading README.md by files, and in Full_Segmentation
         * for .cpp file to comunicaty with .py file
         * */
        static std::string text_file_to_string(const char* path, bool line_sep=true){
            std::string result;
            std::ifstream file(path);
            // Check if the file opened successfully
            if (file.is_open()) {
                std::string line;
                // Read the file line by line
                while (std::getline(file, line)) {
                    if(line != "" && line != " "){
                        result += line;
                        if(line_sep){
                            result += '\n';
                        }
                    }
                }
                // Close the file
                file.close();
            } else {
                std::cerr << "Unable to open the file!" << std::endl;
            }
            return result;
        }
    };


    class FILES{
        public:
        /**
         * Function checks if folder exists, if not, it creates the directory in specified path
         * */
        static void folder_create(const char* folderPath){
                struct stat info;
                if (stat(folderPath, &info) == 0 && (info.st_mode & S_IFDIR)) {
                } else {
                    // Directory does not exist, create it
                    if (mkdir(folderPath, 0777) == 0) {
                        std::cout << "Directory created successfully: " << folderPath << std::endl;
                    } else {
                        std::cerr << "Error: Failed to create directory: " << folderPath << std::endl;
                    }
                }
        }

        /**
         * Function saves image (cv::Mat) to a specified path, returns the name of the file
         * @param inital_path -> Initial path from which the image was opened
         * @param folder_name -> path to folder where the cv::Mat needs to be saved in
         * @param img -> cv::Mat
         * @param save -> If needed only to return the name of the file, switch to false, and no saving will be done, only extraction of the name
         * */
        static std::string save_to_folder(std::string initial_path, std::string folder_name, cv::Mat &img,bool save=true){
                std::string file_name = "";
                int i = initial_path.length() -1;
                //Extract the reversed name of the file from initial path, and it's extension with it,
                //So, from /home/ta/Kornel/001.JPG this will result in string GPJ.1000
                while(initial_path[i] != '/' && initial_path[i] != '\\'){
                    file_name += initial_path[i];
                    i--;
                }
                //Add slash to the end of the name 
                if(folder_name[folder_name.length()-1] != '/' && folder_name[folder_name.length()-1] != '\\'){
                    file_name += '/';
                }
                //Reverse the name
                std::reverse(file_name.begin(),file_name.end());
                //Add a folder path with the name of the file
                file_name = folder_name + file_name;
                //Save
                if(save){
                    cv::imwrite(file_name,img);
                }
                return file_name;
        }

        /**
         *  Function writes data to a specified file by file_path, it can add a separator after the text, and newline
         * */
        static void write_to_file(const char *file_path, std::string line, char separator=';',bool newline = true, bool new_file=false){
            std::FILE* out_file = std::fopen(file_path, new_file ? "w" : "a");
                if (!out_file) {
                    // Handle file open failure
                    std::perror("Error opening file");
                    return;
                }

                // Build the line with the separator
                std::string new_text = line;
                if (separator != '\0') {  // Add separator if provided
                    new_text += separator;
                }

                // Write the line to the file
                if (std::fprintf(out_file, "%s", new_text.c_str()) < 0) {
                    std::perror("Error writing to file");
                    std::fclose(out_file);
                    return;
                }

                // Write a newline if specified
                if (newline) {
                    if (std::fprintf(out_file, "\n") < 0) {
                        std::perror("Error writing newline to file");
                    }
                }

                // Close the file
                std::fclose(out_file);
        }

        /**
         * Function cleares text file in a specified path
         * */
        static void clear_file(const char *file_path){
            std::FILE* out_file;
            out_file = std::fopen(file_path,"w");
            std::fclose(out_file);
        }
    };

    //Class for calculating watershed markers characteristics, like size SD, size mean, excluding 2sigma rule
    class watMarkers{
        public:
        watMarkers(){
            fields.clear();
        }
        
        /*
        *   Add a new field/ marker value
        * */
        void add_value(int field_id){
            //Find the index
            size_t index = 1000000;
            for(size_t x = 0; x < fields.size(); x++){
                if(fields[x].index == field_id){
                    index = x;
                }
            }
            //After having an index
            if(index == 1000000){
                field new_object;
                new_object.count = 1;
                new_object.index = field_id;
                fields.emplace_back(new_object);
                return;
            }
            fields[index].count += 1;
        }

        //Function calculates the mean of segmented area
        double get_mean(){
            double mean;
            mean = 0;
            for(size_t x = 0; x < fields.size(); x++){
                mean += fields[x].count; 
            }
            return mean /fields.size();
        }
        //Function calculates standard deviation of the segmented area
        double get_SD(){
            long double SD = 0, mean = this->get_mean();
            for(size_t x = 0; x < fields.size(); x++){
                SD += pow(double(fields[x].count) - mean,2);
            }
            return sqrt(abs(SD)/double(fields.size()));
        }
        //Function excludes from list areas, which exceed the !2sigma! rule 
        //(here 2sigma rule, as 86% in area size is enough of difference)
        void exclude_borderline_cases(){
            double SD= this->get_SD();
            double mean= this->get_mean();
            std::vector<field> new_fields;
            for(size_t x=0; x < fields.size(); x++){
                const int value = fields[x].count;
                if(abs(mean - value) < 2*abs(SD) && value > 100){
                    new_fields.emplace_back(fields[x]);
                }
            }
            fields = new_fields;
        }
        //Find if index is present in fields
        bool is_present(int field_id){
            for(size_t x = 0; x < fields.size(); x++){
                if(fields[x].index == field_id){
                    return true;
                }
            }
            return false;
        }

        private:

        struct field{
            int index;
            int count;
        };
        std::vector<field> fields;

    };
}