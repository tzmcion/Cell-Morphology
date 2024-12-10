import dearpygui.dearpygui as dpg
import os
import Engine.global_variables as G

class folder_structure:
    def __init__(self,name,paths):
        self.name = name
        self.paths = paths


files_structured = []
paths_list = []

def get_paths_rec(path):
    accepted_types = G.ACCEPTED
    current_folder = G.PATH
    files_path = [f.path for f in os.scandir(path) if not f.is_dir()]
    folders_paths = [f.path for f in os.scandir(path) if f.is_dir()]
    for file_path in files_path:
        extension = file_path[len(file_path)-4:len(file_path)]
        if(extension.upper() not in accepted_types):
            continue
        paths_list.append(file_path)
    for folder in folders_paths:
        get_paths_rec(folder)

def get_paths():
    current_folder = G.PATH
    get_paths_rec(current_folder)
    return paths_list

def load_paths():
    global paths_list
    paths_list = []
    if(G.PATH):
        list = get_paths()
        if(dpg.does_item_exist("SHOW_PATHS")):
            dpg.delete_item("SHOW_PATHS")
        with dpg.window(width=1000, height=400, id="SHOW_PATHS", pos=(0,400)):
            dpg.add_text(default_value=(str("Total Images found: ") + str(len(list))))
            dpg.add_button(label="Continue with following Images:", width=250, height=50,id="APP")
            dpg.bind_item_handler_registry("APP","APP_HANDLER") #FROM ENGINE.APP
            for path in list:
                dpg.add_text(path,color=(255,155,0))

def all_paths():
    with dpg.item_handler_registry(tag="paths_handler") as handler:
        dpg.add_item_clicked_handler(callback=load_paths)
    dpg.bind_item_handler_registry("Save", "paths_handler")

 
