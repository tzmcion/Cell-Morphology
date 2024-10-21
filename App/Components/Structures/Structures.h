#pragma once

#include "../Includes/includes.h"
#include <fstream>
#include <filesystem>

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
            if( &vec == nullptr){
                throw std::invalid_argument("Vector passed to fill is NULL");
            }
            if( list[0] != '[' ){
                throw std::invalid_argument("list passed as argument is not in shape of [ar1, ar2]");
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

        static std::string text_file_to_string(const char* path){
            std::string result;
            std::ifstream file(path);
            
            // Check if the file opened successfully
            if (file.is_open()) {
                std::string line;
                
                // Read the file line by line
                while (std::getline(file, line)) {
                    result += line + "\n";
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

            // Check if the directory exists
            if (std::filesystem::exists(folderPath)) {
            } else {
                // Directory does not exist, create it
                if (std::filesystem::create_directory(folderPath)) {
                } else {
                    std::cerr << "Error: Failed to create directory: " << folderPath << std::endl;
                }
            }
        }
    };
}