import os
import subprocess

#Assuming file is in folder EXECUTIONS

os.chdir("../")

#APP_PATH
APP_PATH = str(os.getcwd())

#Utils path
UTILS = APP_PATH + "/Utils"

#Scan the utils folders and run with ./run.sh

FOLDERS = [f.path for f in os.scandir(UTILS) if f.is_dir()]

for util in FOLDERS:
    run_path = util + "/run.sh"
    if(os.path.exists(run_path)):
        os.chdir(util)
        print(run_path)
        subprocess.run(["./run.sh"])
        os.chdir(UTILS)
    
