// .cpp file for AlgorithmOptions.h
#include "./AlgorithmOptions.h"

AlgorithmOptions::AlgorithmOptions(const char* path_to_file) : ReadOptions("NULL"){
    std::vector<std::string> lines;
    std::ifstream file(path_to_file);   //load the file
    
    //Read the file and copy lines to a vector
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Unable to open the file!" << std::endl;
    }
    std::cout << "HELLO \n";
    //Assuming the file got opened and red
    for(size_t x = 0; x < lines.size(); x++){
        std::string line = this->clear_line(lines[x]);
        //Check if line is empty
        optionObject *object;
        if(line == "" || line==" "){continue;}
        //Check if first character of line declares it is a name
        if(line[0] == '-'){
            object = new optionObject();
            line = line.substr(1, line.size());
            object->name=line;
            continue;
        }
        std::vector<std::string> part_lines = this->split_string(line, ' ');
        char var_type = 'i'; // Default type is int
        const char *letter = (strchr(part_lines[0].c_str(), '<'));
        if(letter != nullptr){
            var_type = std::string(letter + 1)[0];
        }
        std::cout << var_type << std::endl; //Type of variable stored here
        if(var_type == 'i'){
            //integer, apply maximum value to second of vector data
            int value = std::stoi(part_lines[1].c_str());
            object->data.emplace_back(std::make_pair(value,std::numeric_limits<double>::max()));
        }
        else{
            //double, apply maximum value to first of vector data
            double value = std::atof(part_lines[1].c_str());
            object->data.emplace_back(std::make_pair(std::numeric_limits<int>::max(),value));
        }
    }
}

AlgorithmOptions::~AlgorithmOptions(){
    for(size_t x = 0; x < options.size(); x++){
        delete options[x];
    }
}

int AlgorithmOptions::get_int_var(size_t index, std::string name){

}

double AlgorithmOptions::get_db_var(size_t index, std::string name){

}

bool AlgorithmOptions::was_name_found(std::string name){
    
}