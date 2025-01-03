# Process will get the paths of all *.JPG files in directories
# It can also exclude certain paths

import os
import sys

accepted_types = ['.jpg', '.JPG', '.png', '.PNG']
#Change folder to scan
SCAN_PATH = ""
if(len(sys.argv) == 2):
    SCAN_PATH = sys.argv[1]
else:
    SCAN_PATH = "/home/tymoteuszapriasz/KM/HIP/ZPY/segm_out/"
#if no path defined, use default:
#Folder where to look for data
FOLDER_NAME = "segmented"
#Create saving file
save_file = open("paths.csv",'a')

for root,dirs,files in os.walk(SCAN_PATH):
    for file in files:
        file_path = os.path.join(root,file)
        if(FOLDER_NAME in file_path):
            extension = file_path[len(file_path)-4:len(file_path)]
            if(extension not in accepted_types):
                continue
            file_path += ";\n"
            save_file.write(file_path)

save_file.close()