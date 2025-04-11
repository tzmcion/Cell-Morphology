// .cpp file for AlgorithmOptions.h
#include "./AlgorithmOptions.h"

AlgorithmOptions::AlgorithmOptions(const char* options_file_path) : ReadOptions("NULL"){
    std::vector<std::string> lines;
    std::ifstream file(options_file_path);   //load the file
    //Read the file and copy lines to a vector
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open the file: " << options_file_path << std::endl;
        std::invalid_argument("Could not open the provided file for AlgorithmOptions");
    }
    //Assuming the file got opened and red
    optionObject *object = nullptr;
    for(size_t x = 0; x < lines.size(); x++){
        std::string line = this->clear_line(lines[x]);
        //Check if line is empty
        if(line == "" || line==" "){continue;}
        //Check if first character of line declares it is a name
        if(line[0] == '-'){
            if(object != nullptr){
                options.push_back(object);
            }
            object = new optionObject();
            line = line.substr(1, line.size());
            object->name=line;
            continue;
        }
        if(object == nullptr){
            //Throw error for mallfunction of .option file
            this->error_message(options_file_path,line,static_cast<int>(x));
        }
        std::vector<std::string> part_lines = this->split_string(line, ' ');
        char var_type = 'i'; // Default type is int
        const char *letter = (strchr(part_lines[0].c_str(), '<'));
        if(letter != nullptr){
            var_type = std::string(letter + 1)[0];
        }
        if(var_type == 'i'){
            //integer, apply maximum value to second of vector data
            int value = std::stoi(part_lines[1].c_str());
            object->data.push_back(std::make_pair(value,AlgorithmOptions::MAX_DB));
        }
        else{
            //double, apply maximum value to first of vector data
            double value = std::atof(part_lines[1].c_str());
            object->data.push_back(std::make_pair(AlgorithmOptions::MAX_INT,value));
        }
        //Extract the name
        std::string option_name = "";
        for(int x = 0; part_lines[0][x] != '<'; x++){
            option_name += part_lines[0][x];
        }
        object->names.push_back(option_name);

    }
    //End of loop, add what was there
    options.push_back(object);
    //End of this file
}
//
//
//
AlgorithmOptions::~AlgorithmOptions(){
    for(size_t x = 0; x < options.size(); x++){
        delete options[x];
    }
}
//
//
//
size_t AlgorithmOptions::_get_name_index(std::string name){
    for(size_t x = 0; x < options.size(); x++){
        if(name == options[x]->name){
            return x;
        }
    }
    //If not found, throw error
    std::string make_error_msg = "Trying to read or alter non existing class: " + name;
    std::invalid_argument(make_error_msg.c_str());
    return 0;
}
//
//
//
int AlgorithmOptions::get_int_var(size_t index, std::string name){
    size_t idx_opt = _get_name_index(name);
    if(options[idx_opt]->data[index].first == AlgorithmOptions::MAX_INT){
        std::invalid_argument("Trying to get value of incorrect option, d_type should be <double> not <int>");
    }
    return options[idx_opt]->data[index].first;
}
//
//
//
double AlgorithmOptions::get_db_var(size_t index, std::string name){
    size_t idx_opt = _get_name_index(name);
    if(options[idx_opt]->data[index].second == AlgorithmOptions::MAX_DB){
        std::invalid_argument("Trying to get value of incorrect option, d_type should be <int> not <double>");
    }
    return options[idx_opt]->data[index].second;
}
//
//
//
void AlgorithmOptions::set_int_value(size_t index, std::string name, int new_value){
    size_t idx_opt = _get_name_index(name);
    if(options[idx_opt]->data[index].first == AlgorithmOptions::MAX_INT){
        std::invalid_argument("Trying to set value of incorrect option, d_type should be <double> not <int>");
    }
    options[idx_opt]->data[index].first = new_value;
}
//
//
//
void AlgorithmOptions::set_db_value(size_t index, std::string name, double new_value){
    size_t idx_opt = _get_name_index(name);
    if(options[idx_opt]->data[index].second == AlgorithmOptions::MAX_DB){
        std::invalid_argument("Trying to set value of incorrect option, d_type should be <int> not <double>");
    }
    options[idx_opt]->data[index].second = new_value;
}
//
//
//
void AlgorithmOptions::save_options_to_file(const char* file_name, std::string addition){
    const std::string TOP_MSG = "# File created automatically via optimization tool \n #PLEASE DO NOT ALTER THE NAMING OR ORDER!";
    //Code
    std::ofstream file(file_name);
    std::cout << "Writing file: " << file_name;
    std::string current_class = "";
    if(file.is_open()){
        file << TOP_MSG.c_str() << std::endl << std::endl;
        for (size_t x = 0; x < options.size(); x++){
            file << '-' << options[x]->name << std::endl;
            for(size_t y = 0; y < options[x]->data.size(); y++){
                std::string make_line = "    ";
                make_line += options[x]->names[y];
                std::pair<int,double> data = options[x]->data[y];
                if(data.first == AlgorithmOptions::MAX_INT){
                    make_line += "<double>: ";
                    make_line += std::to_string(data.second);
                }else{
                    make_line += "<int>: ";
                    make_line += std::to_string(data.first);
                }

                file << make_line << std::endl;
            }
        }
    }
    file << std::endl << addition;
    file.close();
}
//
//
//
size_t AlgorithmOptions::options_size_by_name(std::string name){
    size_t idx_opt = _get_name_index(name);
    return options[idx_opt]->data.size();
}