"""
This file consists of functions used to
select paths
"""
import subprocess
import os

def ask_folder_path() -> str|None:
    try:
        result = subprocess.run(
            ["zenity", "--file-selection", "--directory", "--title=Select Folder"],
            check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        )
        folder_path = result.stdout.decode("utf-8").strip()
        return folder_path
    except subprocess.CalledProcessError:
        return None
    

def study_path(path:str) -> list[str]:
    """
    Detect how many images and subdirectories are in the folder
    """
    DIR = os.getcwd()
    os.chdir(path)
    IMAGES = []
    DIRECTORIES = [f for f in os.listdir() if os.path.isdir(f)]
    for root, dirs, files in os.walk("."):
        for file in files:
            IMAGES.append(file)
    OPTIONS_FILE = [f for f in os.listdir() if os.path.isfile(f) and f.endswith(".option")]
    os.chdir(DIR)
    return [DIRECTORIES,IMAGES, OPTIONS_FILE]

def get_all_images(path:str)-> list[str]:
    DIR = os.getcwd()
    os.chdir(path)
    IMAGES = []
    for root, dirs, files in os.walk("."):
        for file in files:
            if file.endswith((".jpg",".png",".jpeg",".JPG",".PNG",".JPEG")):
                IMAGES.append(file)
    os.chdir(DIR)
    return IMAGES