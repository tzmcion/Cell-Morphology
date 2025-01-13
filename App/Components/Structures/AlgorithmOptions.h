#pragma once
#include "../Includes/includes.h"
#include "./PrepAllOptions.h"

#define DOUBLE_MAX = 9999.9
#define INT_MAX = 99999     //Assuming nobody would use such huuge arguments

class AlgorithmOptions: private ReadOptions{
    public:
    AlgorithmOptions(const char* path_to_file);
    ~AlgorithmOptions();

    int get_int_var(size_t index, std::string name);
    double get_db_var(size_t index, std::string name);

    bool was_name_found(std::string name);

    private:

    struct optionObject{
        std::string name;
        //Depending on data type of variable, either returning will be "first", or "second"
        std::vector<std::pair<int,double>> data;
    };
    std::vector<optionObject*> options;
};