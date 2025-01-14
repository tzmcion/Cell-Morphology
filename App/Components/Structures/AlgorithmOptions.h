#pragma once
#include "../Includes/includes.h"
#include "./PrepAllOptions.h"

class AlgorithmOptions: private ReadOptions{
    public:
    AlgorithmOptions(const char* path_to_file);
    ~AlgorithmOptions();

    int get_int_var(size_t index, std::string name);
    double get_db_var(size_t index, std::string name);

    int options_size_by_name(std::string name);

    size_t get_name_index(std::string name);

    private:

    struct optionObject{
        std::string name;
        //Depending on data type of variable, either returning will be "first", or "second"
        std::vector<std::pair<int,double>> data;
    };
    std::vector<optionObject*> options;
    size_t max_value = std::numeric_limits<size_t>::max();
};