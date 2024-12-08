#include "../../Components/Watershed/Watershed.h"
#include "../../Components/Structures/Colors.h"
#include "../../Components/Transformations/Transformations.h"
#include "../../Components/Structures/Structures.h"
#include "../../Components/Threading/Threads.h"

/**
 *  Component Created by Tymoteusz Apriasz
 *  ©Avant 2024/2025
 *  Process: Counting of total cell surface on image
 *  Args:  
 *  Total of 3 obligatory arguments
 *  @param option_files path to option files, can be "def" to use default options
 *  @param image_list list of paths of images
 *  @param output_folder path to output folder
 *  TEMPLATE: ./main.out def [/home/im1.JPG,/home/im2.JPG] /home/work/output_surface
 * */

int main(int argc, char** argv){
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if(argc != 4){
        throw std::invalid_argument("Number of argumnets is invalid, required is 3");
    }
    const char* OPTIONS_PATH = argv[1];
    const char* PATHS = argv[2];
    const char* OUT_FOLDER = argv[3];
    const std::string INP_DATA = Entites::Convert::text_file_to_string("../Count_Surface/README.md");
    std::cout << Colors::BG_BRIGHT_GREEN << INP_DATA << Colors::RESET;
    std::cout << "-------------------------------------------------- \n";
    std::cout << "Reading input files, note that this process will end in infinite loop if provided array does not end with \"]\"! \n";
    std::vector<std::string> images;
    Entites::Convert::c_char_to_string(images,PATHS);
    for(size_t x = 0; x < images.size(); x++){
        std::cout << "Image_" << x << ' ' << Colors::MAGENTA << images[x] << Colors::RESET << std::endl;
    }
    std::cout << "Detected " << images.size() << " files to convert in total \n";
    std::cout << "Output folder is: " << Colors::YELLOW <<  OUT_FOLDER << Colors::RESET << std::endl;
    Entites::FILES::folder_create(OUT_FOLDER);
    std::cout << Colors::RED << "Reading options from provided file" << Colors::RESET << std::endl;
    const std::string out_file = std::string(OUT_FOLDER) + "/DATA" +"_WATERSHED" + ".csv";
    Entites::FILES::clear_file(out_file.c_str());
    //TODO
    //NOW DO FOR DEFAULT VALUES
    size_t cores = std::thread::hardware_concurrency();
    if(cores == 0){
        cores = 3;
    }
    Threading threads(cores);
    for(size_t x = 0; x < images.size(); x++){
        //INCLUDE FOR MULTITHREADING, OR SIE ZESRASZ
        threads.enqueueTask([x,OUT_FOLDER,out_file,images](){
            const std::string PATH = images[x];
            std::cout << Colors::YELLOW << "Processing of:: " << Colors::RESET << PATH;
            std::cout << std::flush;
            cv::Mat image;
            image = cv::imread(PATH, cv::IMREAD_GRAYSCALE);
            //
            //ALGORYTHM
            //
            cv::Mat background_mask, foreground_mask, foreground_regions,markers;
            Watershed::background_mask(image,background_mask);
            Watershed::foreground_regions(image,foreground_regions,background_mask);
            Watershed::foreground_mask(image,foreground_mask,foreground_regions,background_mask);
            cv::connectedComponents(foreground_mask,markers);
            markers.setTo(1, background_mask == 0); // Ensure background is labeled as 1
            markers.setTo(0, (background_mask != 0) & (foreground_mask == 0)); // Unknown regions are 0
            cv::Mat to_wat;
            image.copyTo(to_wat);
            cv::Mat image_watershed, result, watershed_markers;
            Transformations::opening(to_wat,to_wat,'O',3);
            Transformations::double_blur(to_wat,to_wat,3,3);
            Watershed::clache(to_wat,to_wat,3.0);
            cv::medianBlur(to_wat,to_wat,3);
            cv::cvtColor(to_wat, image_watershed, cv::COLOR_GRAY2BGR);
            markers.convertTo(watershed_markers,CV_32S);
            cv::watershed(image_watershed,watershed_markers);
            cv::cvtColor(image,result,cv::COLOR_GRAY2RGB);
            //I have markers, now I need to somehow get all the unique ones
            Entites::watMarkers markers_counter;
            for (int i = 0; i < markers.rows; i++) {
                for (int j = 0; j < markers.cols; j++) {
                    int markerValue = watershed_markers.at<int>(i, j);
                    if(markerValue >= 2) {
                        markers_counter.add_value(markerValue);
                    }
                }
            }
            //Now I need to get all the unique markers, find the average area size
            markers_counter.exclude_borderline_cases();
            for (int i = 0; i < markers.rows; i++) {
                for (int j = 0; j < markers.cols; j++) {
                    int markerValue = watershed_markers.at<int>(i, j);
                    if (markers_counter.is_present(markerValue)) {
                        result.at<Vec3b>(i, j) = Vec3b(markerValue*50 +50,200,markerValue*25 + 100);
                    }
                    else{
                        result.at<Vec3b>(i, j) = Vec3b(0, 0, 0); // Blue for background
                    }
                }
            }

            cv::Mat labels, stats, centroids, markers_for_cc;
            markers_for_cc = (watershed_markers > 1)*255; 
            int numLabels = cv::connectedComponentsWithStats(markers_for_cc, labels, stats, centroids, 4, CV_32S);
            //Crerate folder for segmentation
            std::string add = PATH;
            add.erase(0,add.rfind('/')+1);
            add.erase(add.rfind('.'));
            std::cout << "PATH: " << add << std::endl; 
            std::string OUT_FOLDER_SEG = std::string(OUT_FOLDER) + "/segmented" + add;
            Entites::FILES::folder_create(OUT_FOLDER_SEG.c_str());
            for(int label=1; label < numLabels; label++){
                //Add 10 pixels everytime
                int x = stats.at<int>(label, cv::CC_STAT_LEFT) - 20;
                int y = stats.at<int>(label, cv::CC_STAT_TOP) - 20;
                int width = stats.at<int>(label, cv::CC_STAT_WIDTH) + 40;
                int height = stats.at<int>(label, cv::CC_STAT_HEIGHT) + 40;
                if(x < 0)x=0;
                if(y < 0)y=0;
                if(x + width >= image.cols){width = image.cols-x;}
                if(y + height >= image.rows){height = image.rows-y;}

                // Crop the bounding box from labels and the original image
                cv::Mat croppedLabels = labels(cv::Rect(x, y, width, height));
                cv::Mat croppedImage = image(cv::Rect(x, y, width, height));

                // Create a mask for the current component within the cropped region
                cv::Mat compMask = (croppedLabels == label);
                compMask.convertTo(compMask, CV_8U);
                Transformations::dilation(compMask,compMask,3,2,3);
                // Apply the mask to the cropped image
                cv::Mat result;
                croppedImage.copyTo(result, compMask);

                // Calculate brightness using your custom function
                double brightness = Transformations::image_brightnes(image);

                // Convert brightness to a valid uchar value
                uchar brightnessValue = cv::saturate_cast<uchar>(brightness);

                // Replace all 0 pixels in croppedImage with brightnessValue
                result.setTo(brightnessValue, result == 0);
                Transformations::square_and_resize(result,224);
                // Display the cropped component
                //Create folder for output and save to output
                //
                std::string file_name = "/IMG" + std::to_string(label) + ".JPG";
                Entites::FILES::save_to_folder(file_name,OUT_FOLDER_SEG,result);
                // cv::imshow("Cropped Component", result);
                // cv::waitKey(0);
            }

            Mat imageBGRA;
            cvtColor(image, imageBGRA, COLOR_BGR2RGB);

            Mat blended;
            addWeighted(imageBGRA, 0.80, result, 0.20, 0, blended);
            // cv::imshow("DD",blended);
            // cv::waitKey(0);
            std::string mean_str = std::to_string(markers_counter.get_mean());

            Entites::FILES::write_to_file(out_file.c_str(),PATH, ';',false);
            Entites::FILES::write_to_file(out_file.c_str(),std::to_string(markers_counter.get_mean()),';',false);
            Entites::FILES::write_to_file(out_file.c_str(),std::to_string(markers_counter.get_SD()),';');
            //
            //END OF ALGORYTHM
            //
            std::cout << Colors::GREEN <<" [...DONE! ]"<< Colors::RESET << " Saving File... ";
            std::string out_name = Entites::FILES::save_to_folder(PATH,OUT_FOLDER,blended);
            std::cout << Colors::GREEN <<" [...DONE! ]" << Colors::RESET << " Succesfully saved to: " << Colors::MAGENTA << out_name << Colors::RESET << std::endl;
        });
    }
    std::string out_data = Entites::Convert::text_file_to_string("../SUCCES.txt");
    std::cout << out_data << "All operations finished, process will end with zero" << std::endl << std::endl;
    return 0;
}