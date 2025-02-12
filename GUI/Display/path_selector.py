"""
This file consists of functions used to
select paths
"""
import subprocess

def ask_folder_path():
    try:
        result = subprocess.run(
            ["zenity", "--file-selection", "--directory", "--title=Select Folder"],
            check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        )
        folder_path = result.stdout.decode("utf-8").strip()
        return folder_path
    except subprocess.CalledProcessError:
        return None