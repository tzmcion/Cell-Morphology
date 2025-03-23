// .cpp file for AlgorithmOptions.h
#include "./AlgorithmOptions.h"

AlgorithmOptions::AlgorithmOptions(const char* path_to_file) : ReadOptions("NULL"){
    std::vector<std::string> lines;
    std::ifstream file(path_to_file);   //load the file
    std::cout << "READING: " << path_to_file << std::endl;
    //Read the file and copy lines to a vector
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open the file: " << path_to_file << std::endl;
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
            this->error_message(path_to_file,line,static_cast<int>(x));
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
            object->data.push_back(std::make_pair(value,std::numeric_limits<double>::max()));
        }
        else{
            //double, apply maximum value to first of vector data
            double value = std::atof(part_lines[1].c_str());
            object->data.push_back(std::make_pair(std::numeric_limits<int>::max(),value));
        }
    }
    //End of loop, add what was there
    options.push_back(object);
    //End of this file
}

AlgorithmOptions::~AlgorithmOptions(){
    for(size_t x = 0; x < options.size(); x++){
        delete options[x];
    }
}

/**
 *  !MUST BE THE SAME SIZE uppercase or lowercase or whatever
 * */
size_t AlgorithmOptions::get_name_index(std::string name){
    for(size_t x = 0; x < options.size(); x++){
        if(name == options[x]->name){
            return x;
        }
    }
    return this->max_value;
}

int AlgorithmOptions::get_int_var(size_t index, std::string name){
    size_t idx_opt = get_name_index(name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    return options[idx_opt]->data[index].first;
}

double AlgorithmOptions::get_db_var(size_t index, std::string name){
    size_t idx_opt = get_name_index(name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    return options[idx_opt]->data[index].second;
}

int AlgorithmOptions::options_size_by_name(std::string name){
    size_t idx_opt = get_name_index(name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    return options[idx_opt]->data.size();
}

double AlgorithmOptions::get_next_db(std::string name){
    std::string c_name = name == "" ? curr_name : name;
    size_t idx_opt = this->get_name_index(c_name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    options[idx_opt]->iterator += 1;
    return options[idx_opt]->data[options[idx_opt]->iterator-1].second;
}

int AlgorithmOptions::get_next_int(std::string name){
    std::string c_name = name == "" ? curr_name : name;
    size_t idx_opt = this->get_name_index(c_name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    options[idx_opt]->iterator += 1;
    return options[idx_opt]->data[options[idx_opt]->iterator-1].first;
}

void AlgorithmOptions::set_int_value(size_t index, std::string name, int new_value){
    size_t idx_opt = get_name_index(name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    options[idx_opt]->data[index].first = new_value;
}

void AlgorithmOptions::set_db_value(size_t index, std::string name, double new_value){
    size_t idx_opt = get_name_index(name);
    if(idx_opt == this->max_value){
        throw std::invalid_argument("Provided name for algorithm does not match name in .option file provided");
    }
    options[idx_opt]->data[index].second = new_value;
}

void AlgorithmOptions::save_options_to_file(const char* file_name){
    //TODO
}

size_t AlgorithmOptions::get_option_index(std::string class_name, std::string searched_name){
    //Currently not supported
    return 0;
}