#pragma once
#include "../Includes/includes.h"
#include "./PrepAllOptions.h"

/**
 * Class for managing options for specified file .aloption 
 * each option can be accessed by it's CLASS NAME and INDEX IN THAT CLASS
 * Therefore, the order of option can not be changed
 */
class AlgorithmOptions: private ReadOptions{
    public:
    /**
     * Basic constructor
     * @param path_to_file path to .options file which will be used
     */
    AlgorithmOptions(const char* options_file_path);

    /**
     * Default constructor !! Don't use unless valid reason
     */
    AlgorithmOptions(bool val1, bool val2) : ReadOptions("NULL"){
        is_default = true;
    }
    /*Destructor for this class*/
    ~AlgorithmOptions();

    /**
     * Method returns the integer value of option
     * @param index Index of the option n it's class
     * @param name name of the class
     */
    int get_int_var(size_t index, std::string name);

    /**
     * Method returns the double value of option
     * @param index Index of the option n it's class
     * @param name name of the class
     */
    double get_db_var(size_t index, std::string name);

    //For Editing of the options

    /**
     * Function allows to set the value for <double> d_type
     * @param index index of the option in it's class
     * @param name name of the class in aloptions file
     * @param new_value new value to be set
     */
    void set_db_value(size_t index, std::string name, double new_value);

    /**
     * Function allows to set the value for <int> d_type
     * @param index index of the option in it's class
     * @param name name of the class in aloptions file
     * @param new_value new value to be set
     */
    void set_int_value(size_t index, std::string name, int new_value);

    /**
     * Function saves current state of options to a provided file
     * The saved file will automatically have a comment stating it was generated automatically
     * @param file_path path to file, be carefoul with local paths, it is better to generate global path
     * @param addition additional text added at the end of options. Mainly used to generate order of options in automatization
     */
    void save_options_to_file(const char* file_path, std::string addition="");

    /**
     * Function returns count of options which has a certain class
     * So, if in the file class "WATERSHED" has 5 options, it will return 5, if the options are written correctly
     * @param name Name of the class which number of options needs to be known
     */
    size_t options_size_by_name(std::string name);

    static constexpr double MAX_DB = std::numeric_limits<double>::max();
    static constexpr int MAX_INT = std::numeric_limits<int>::max();
    bool is_default = false;

    //Default AlgorithmOptions object for Optimiation and usage somewhere
    static AlgorithmOptions& Defaulf(){
        static AlgorithmOptions default_values(true,true);
        return default_values;
    }

    private:

        
    /*Function returns the index of class in options vector*/
    size_t _get_name_index(std::string name);


    /**
     * optionObject is structured as follows, it has the name of the class, and it's elements stored in twin vectors
     * one vector stores the name of the option, and the other stores it's values
     * the options are accesed by the name of the class (param name) and by index of the option (param data).
     * @param name Stores the name of the option class
     * @param data Vector of pairs which stores the value of the option
     * @param names Vector of names of the options in class, same size as data
     */
    struct optionObject{
        std::string name;
        std::vector<std::pair<int,double>> data; //Depending on data type of variable, either returning will be "first", or "second"
        std::vector<std::string> names;
    };

    //Main variable which stores the options, initiallized in constructor
    std::vector<optionObject*> options;
};