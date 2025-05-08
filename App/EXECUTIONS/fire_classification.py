import os
import subprocess

FOLDER = str(os.getcwd())
#Define where the images are for segmentation
IMAGES = FOLDER + "/OUT"

os.chdir('../')

APP_PATH = str(os.getcwd())

#Path to a cpp file which runs other cpp files
PREPROCESS = APP_PATH + '/Utils/Classification_with_masks'
#path to file with options
OPTIONS = FOLDER + '/options_algorithms.option'

#Path to out directory, it must exist
OUT_DIR = FOLDER + '/OUT/'

#EXCLUDED FOLDERS BY NAME AND ID
EXCLUDED_FOLDERS = []


for index in range(0,len(EXCLUDED_FOLDERS)):
    EXCLUDED_FOLDERS[index] = IMAGES + '/' + EXCLUDED_FOLDERS[index]

#Excluded only by one character characterisitic for a folder
EXCLUDED_FOLDERS += ["A","B"]

def get_paths(folder:str, name_starts = ""):
    result = ""
    accepted_types = ['.jpg', '.JPG', '.png', '.PNG', '.bmp', '.BMP']
    result += '['
    folder_path = [f.path for f in os.scandir(folder) ]
    coma = False
    for file_path in folder_path:
        if name_starts != "":
            if(file_path.find(name_starts) == -1):
                continue
        extension = file_path[len(file_path)-4:len(file_path)]
        global_path = ""
        if(extension not in accepted_types):
            continue
        if(coma):
            global_path += ','
        coma = True
        global_path += "/" + str(file_path[1:len(file_path)])
        result += global_path
    result += ']'
    return result


paths = [f.path for f in os.scandir(IMAGES) if f.is_dir()]
IMG_PATH = FOLDER + "/temp_path.txt"
for path in paths:
    if (path in EXCLUDED_FOLDERS) or (path[-3:-2].strip() in EXCLUDED_FOLDERS) or (path[-2:].strip() in EXCLUDED_FOLDERS):
        continue
    deep_paths = [f.path for f in os.scandir(path)]
    OUT_DD = OUT_DIR + path[len(path)-3:len(path)] + "/CLASSIFICATION"
    print(OUT_DD)
    IMAGES = (get_paths(path))
    BG_MASKS = (get_paths(str(path + "/Fast_Execution/MASKS"),"BACKGROUND_"))
    FG_MASKS = (get_paths(str(path + "/Fast_Execution/MASKS"),"FOREGROUND_"))
    os.chdir(PREPROCESS)
    result = subprocess.run(["./main.out", OPTIONS, IMAGES, BG_MASKS, FG_MASKS, OUT_DD])
    os.chdir(APP_PATH)
    
    
