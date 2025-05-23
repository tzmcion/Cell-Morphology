/**
 * This file has a set of distribution functions, which help to save and display them.
 */
#pragma once
#include "../Includes/includes.h"
#include "../Structures/Structures.h"

class Mesh{
    /**
     * Class Mesh converts a given distribution to a mesh of distributions, by creating a avg 
     * of single 
     */
    public:
        Mesh(){}
        ~Mesh(){}

        /**
         * Function deletes the previous mesh, and creates new one
         * @param nr_of_elements - Size of the vector, which will be in mesh
         */
        void create_mesh(size_t nr_of_elements){
            data.clear();
            data.resize(0);
            for(size_t x = 0; x < nr_of_elements; x++){
                data.push_back(0);
            }
        }

        /**
         * mesh is empty
         */
        bool is_empty(){
            return data.size() == 0;
        }

        /**
         * Function adds data to the mesh
         * @param new_data - vector which needs to be meshed
         */
        void add_to_mesh(std::vector<int>& new_data){
            if(new_data.size() != data.size()){
                std::cout << "NEW SIZE:" << new_data.size() << " OLD SIZE: " << data.size() << std::endl;
                throw std::invalid_argument("Size of vector to mesh is not equal to provided earlier nr_of_elements");
            }
            for(size_t x = 0; x < data.size(); x++){
                data[x] += new_data[x];
            }
            mesh_count += 1;
        }

        /**
         * Function calculates the mesh, and saves it to the .csv file
         * @param path_to_save - Path where the file will be saved
         * @param min_value - Minimum value to create start the x axis of the distribution
         * @param max_value - Maximum value to create end of the x axis of the dirstribution
         */
        void save_mesh_to_csv(const char* path_to_save, double min_value, double max_value){
            for(size_t x = 0; x < data.size(); x++){
                double val = (std::round((min_value + max_value*(x/double(data.size()))) * 100.0) / 100.0)*PIXEL_TO_RADIUS;
                Mesh::write_to_file(path_to_save,std::to_string(val),';',false);
                Mesh::write_to_file(path_to_save,std::to_string(data[x]/mesh_count),' ',true);
            }
        }
    private:
        std::vector<int> data;
        int mesh_count = 0;

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
};