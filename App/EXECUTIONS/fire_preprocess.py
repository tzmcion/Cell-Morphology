import os
import subprocess

FOLDER = str(os.getcwd())

os.chdir('../')
os.chdir('../')
IMAGES = str(os.getcwd()) + "/NOR_org"

APP_PATH = str(os.getcwd())

PREPROCESS = APP_PATH + '/App/Utils/Preprocess_all'
OPTIONS = FOLDER + '/settings.option'

#This folder must exist!
OUT_DIR = FOLDER + '/test_test'

#EXCLUDED FOLDERS!!!
#! NOT EXCLUDED A10
EXCLUDED_FOLDERS = []

for index in range(0,len(EXCLUDED_FOLDERS)):
    EXCLUDED_FOLDERS[index] = IMAGES + '/' + EXCLUDED_FOLDERS[index]

#Excluded only by one character characterisitic for a folder
EXCLUDED_FOLDERS += ["B","D","H","01","02","03","04","06","07","08","10","11"]

def get_paths(folder:str):
    result = ""
    accepted_types = ['.jpg', '.JPG', '.png', '.PNG']
    result += '['
    folder_path = [f.path for f in os.scandir(folder) ]
    coma = False
    for file_path in folder_path:
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
    OUT_DD = OUT_DIR + '/' + path[len(path)-3:len(path)]
    f = open(IMG_PATH, "w")
    f.write(get_paths(path))
    f.close()
    os.chdir(PREPROCESS)
    result = subprocess.run(["./main.out", OPTIONS, IMG_PATH, OUT_DD])
    os.chdir(APP_PATH)
    

