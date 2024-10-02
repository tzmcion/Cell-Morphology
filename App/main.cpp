#include "./Components/Transformations.h"

//Transformation of set of images given in a string input, and smoothen of the brightness

int main(int argc, char** argv) {
    if(argc < 2){
        throw std::invalid_argument("SubComponent requires list of images to calculate mean");
    }
    const std::string comp_list = argv[1];
    std::cout << comp_list;
    // const double brightnes = Transformations::image_brightnes("./Test/T1.JPG");
    // std::cout << "Average brightnes of the image is: " << brightnes << std::endl;
    // Transformations::alter_brightnes("./Test/T1.JPG",brightnes);
    return 0;
}