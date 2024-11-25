# Process will get the paths of all *.JPG files in directories
# It can also exclude certain paths

import os
import shutil
from pathlib import Path


accepted_types = ['.jpg', '.JPG', '.png', '.PNG']
current_folder = ["./out/B07"]
print('[', end='')
paths = [f.path for f in os.scandir("./out") if f.is_dir()]

for path in paths:
    if path not in current_folder:
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