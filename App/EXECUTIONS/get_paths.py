# Process will get the paths of all *.JPG files in directories
# It can also exclude certain paths

import os
import shutil
from pathlib import Path

#Here define the path from which the 
PATH_TO_GET = "./preprocessed"

#Here define accepted types of images
accepted_types = ['.jpg', '.JPG', '.png', '.PNG']
#Here define the one folder we need paths from, or leave empty to search whole directory for images
current_folder = ["A10"]
print('[', end='')
paths = [f.path for f in os.scandir(PATH_TO_GET) if f.is_dir()]

def check_curr_folder():
    cf = []
    for i in current_folder:
        cf.append(PATH_TO_GET + "/" + i)
    return cf

for path in paths:
    if len(current_folder) > 0 and path not in check_curr_folder():
        continue
    folder_path = [f.path for f in os.scandir(path) ]
    coma = False
    for file_path in folder_path:
        extension = file_path[len(file_path)-4:len(file_path)]
        global_path = ""
        if(extension not in accepted_types):
            continue
        if(coma):
            global_path += ','
        coma = True
        global_path += str(os.getcwd() + file_path[1:len(file_path)])
        print(global_path, end='')
print(']', end='')