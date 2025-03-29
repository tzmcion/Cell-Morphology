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

    //For Editing of the options

    /**
     * TODO: Function will allow altering of double option
     */
    void set_db_value(size_t index, std::string name, double new_value);
    /**
     * TODO: Function will allow altering of integer option
     */
    void set_int_value(size_t index, std::string name, int new_value);
    /**
     * TODO: Function will generate the output .option file
     */
    void save_options_to_file(const char* file_path, std::string addition="");

    void set_name_for_iterations(std::string new_name){
        this->curr_name = new_name;
    }

    int options_size_by_name(std::string name);

    /*For class of option*/
    size_t get_name_index(std::string name);
    /*For subclass*/
    size_t get_option_index(std::string class_name, std::string searched_name);

    static constexpr double MAX_DB = std::numeric_limits<double>::max();
    static constexpr int MAX_INT = std::numeric_limits<int>::max();

    private:

    struct optionObject{
        std::string name;
        //Depending on data type of variable, either returning will be "first", or "second"
        std::vector<std::pair<int,double>> data;
        std::vector<std::string> names;
        size_t iterator = 0;
    };
    std::vector<optionObject*> options;
    size_t max_value = std::numeric_limits<size_t>::max();
    std::string curr_name = "";
};