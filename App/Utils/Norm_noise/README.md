Component Created by Tymoteusz Apriasz
©Avant 2024/2025
Process Reducing of the noise on the image using connected components
Args:  
Total of 8 params
@param threshold Threshold value for detecting the object as removable (ratio -- mean/25 * threshold_black)
@param area_size size of detected object being to big to be counted as noise
@param kernel_size_dilation_initial size of initial dilation, before the painting of gray over the detected object 
@param kernel_size_dilation_second size of second dilation, after the painting of gray over the detected object
@param inpaint_size size of kernel of inpaint, inpaint merges the objects with background
@param inpaint_type type of inpaint algorithm, 1 or 0
@param input_images_list - list of images to process
@param output_folder_dir - directory of output folder
Example of input: ./main.out 15 30 3 5 5 1 [./d1.jpg,./d2.jpg,./d3.jpg,./d4.jpg] ./temp_img

!! WARNING DO NOT USE SPACES WHEN PROVIDING PATHS! INCORRECT EXAMPLE: [./d1.jpg, ./d2.jpg] !!

NOTE! Please be aware that optimal size for mage is 1000x1000. Biggest size is hugely ineffective and requires too much time to process.
Please use Resize process provided by author, to make image compatibile with algorithms
NOTE! Please use grayscale images, if image is not saved as grayscale, it might not work properly