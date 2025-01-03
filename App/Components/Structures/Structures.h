#pragma once

#include "../Includes/includes.h"
#include <fstream>
#include <sys/stat.h>

namespace Entites{
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
         *  Function converts list in shape of [ar1, ar2, ar3] to vector
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

        static std::string save_to_folder(std::string initial_path, std::string folder_name, cv::Mat &img,bool save=true){
                std::string file_name = "";
                int i = initial_path.length() -1;
                while(initial_path[i] != '/' && initial_path[i] != '\\'){
                    file_name += initial_path[i];
                    i--;
                }
                if(folder_name[folder_name.length()-1] != '/' && folder_name[folder_name.length()-1] != '\\'){
                    file_name += '/';
                }
                std::reverse(file_name.begin(),file_name.end());
                file_name = folder_name + file_name;
                if(save){
                    cv::imwrite(file_name,img);
                }
                return file_name;
        }

        static void write_to_file(const char *file_path, std::string line, char separator=';',bool newline = true){
            std::string new_text = line + separator;
            std::FILE* out_file;
            out_file = std::fopen(file_path,"a");
            std::fprintf(out_file,new_text.c_str());
            if(newline){
                std::fprintf(out_file,"\n");
            }
            std::fclose(out_file);
        }

        static void clear_file(const char *file_path){
            std::FILE* out_file;
            out_file = std::fopen(file_path,"w");
            std::fclose(out_file);
        }
    };

    class watMarkers{
        public:
        watMarkers(){
            fields.clear();
        }
        
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

        //The distribution (histogram) should be simillar to normal distribution, therefore
        //We shall use 3SD rule to exclude borderline cases
        double get_mean(){
            double mean;
            mean = 0;
            for(size_t x = 0; x < fields.size(); x++){
                mean += fields[x].count; 
            }
            return mean /fields.size();
        }
        //SD
        double get_SD(){
            long double SD = 0, mean = this->get_mean();
            for(size_t x = 0; x < fields.size(); x++){
                SD += pow(double(fields[x].count) - mean,2);
            }
            return sqrt(abs(SD)/double(fields.size()));
        }
        //Excluding 3sigma rule, here will be 2sigma rule :)
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