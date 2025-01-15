#pragma once
#include "../Includes/includes.h"
#include "./PrepAllOptions.h"


//TODO Describe all those options
class AlgorithmOptions: private ReadOptions{
    public:
    AlgorithmOptions(const char* path_to_file);
    ~AlgorithmOptions();

    int get_int_var(size_t index, std::string name);
    double get_db_var(size_t index, std::string name);

    double get_next_db(std::string name = "");
    int get_next_int(std::string name = "");

    void set_name_for_iterations(std::string new_name){
        this->curr_name = new_name;
    }

    int options_size_by_name(std::string name);

    size_t get_name_index(std::string name);

    private:

    struct optionObject{
        std::string name;
        //Depending on data type of variable, either returning will be "first", or "second"
        std::vector<std::pair<int,double>> data;
        size_t iterator = 0;
    };
    std::vector<optionObject*> options;
    size_t max_value = std::numeric_limits<size_t>::max();
    std::string curr_name = "";
};