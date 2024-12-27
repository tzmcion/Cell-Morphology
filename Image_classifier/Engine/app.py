import dearpygui.dearpygui as dpg
import Engine.global_variables as G
import shutil
import os
from Engine.display_paths import get_paths


def can_convert_to_int(value):
    try:
        int(value)  # Try converting to integer
        return True
    except ValueError:  # If conversion fails, it's not a valid integer
        return False

def app():
    MODE = G.MODE
    OUT = G.OUTPUT
    BINDINGS = G.BINDERS
    PATHS = get_paths()
    #Delete all windows
    for item in dpg.get_all_items():
        if(dpg.does_item_exist(item)):
            if dpg.get_item_type(item) == "mvAppItemType::mvWindowAppItem":
                dpg.delete_item(item)
    #Delete window if exists
    if(dpg.does_item_exist("Binds")):
        dpg.delete_item("Binds")
    #Define winndow for classes and for key binding
    with dpg.window(id="Binds", label="Classes and Bindings", pos=(0,0), width=1000, height=150):
        dpg.add_text("Here is a binder, whenever a photo appears, if the focus is on window with image, it will wait for user to press one of binded keys")
        dpg.add_text("Then, it will move/copy the image to a folder with the name of class binded to the key")
        dpg.add_text("Please select name of the classes and binding keys, supported are numbers 0-9, and space", color=(200,100,50))
        with dpg.group(id="Bindings",horizontal=True):
            idx = 0
            for bind in BINDINGS:
                with dpg.group():
                    dpg.add_input_text(default_value=bind.name, label="Class Name", width=100, callback=change_bind_key, id=str("B-" + str(idx)))
                    dpg.add_input_text(default_value=bind.key, label="Key binded", width=100, callback=change_bind_key, id=str("K-" + str(idx)))
                    idx+=1

    with dpg.window(no_move=True,no_collapse=True,no_close=True,id="Images",pos=(100,150),label="Display Images",width=800,height=800):
        with dpg.group(horizontal=False):
            dpg.add_text("0", id="INDEX_IMG")
            IDX = int(dpg.get_value("INDEX_IMG"))
            dpg.add_text(PATHS[IDX], id="Image_Path")
            dpg.add_text(str("MODE: "))
            dpg.add_text(MODE, color=(255,0,0))
        render_image(PATHS[IDX])
    with dpg.handler_registry():
        dpg.add_key_press_handler(callback=image_key_press_handler, user_data="Images")


def change_bind_key(sender,value):
    idx = int(sender[2:len(sender)])
    if(sender[0] == 'B'):
            G.BINDERS[idx].name = value
    if(sender[0] == 'K'):
        if not(can_convert_to_int(value) and str(value).capitalize() != "SPACE"):
            return
        if(str(value).capitalize() == "SPACE"):
            G.BINDERS[idx].key = -1
        else:
            G.BINDERS[idx].key = int(value)

def render_image(path):
    if(dpg.does_item_exist("img_txt")):
        dpg.delete_item("img_txt")
    if(dpg.does_item_exist("texture")):
        dpg.delete_item("texture")
    with dpg.texture_registry(show=False, id="texture"):
        width, height, channels, data = dpg.load_image(path)
        dpg.add_static_texture(width, height, data, tag="image_texture")
    dpg.add_image("image_texture",id="img_txt", width=700, height=700, parent="Images", pos=(50,100))  # Adjust width and height as desired


def copy_file_to(inp,dst,idx):
    extension = '.jpg'
    if(os.path.exists(dst)):
        shutil.move(inp, dst + '/' + str(idx) + extension)
        return
    os.mkdir(dst)
    shutil.move(inp, dst + '/' + str(idx) + extension)

def image_key_press_handler(sender,value):
    PATHS = get_paths()
    COPY_PATH = G.OUTPUT
    INP_PATH = dpg.get_value("Image_Path")
    print(INP_PATH)
    ADD_P = "None"
    key = 0
    iterator = 0
    if(value == 524):
        key = -1
    else:
        key = value - 536
    for index in range(0,len(G.BINDERS)):
        if str(G.BINDERS[index].key) == str(key):
            ADD_P = G.BINDERS[index].name
            iterator = G.BINDERS[index].iterator
            G.BINDERS[index].iterator += 1
    if(ADD_P == "None"):
        return
    IDX = int(dpg.get_value("INDEX_IMG"))
    copy_file_to(INP_PATH, COPY_PATH + '/' + ADD_P,str(str(IDX) + "_" + str(iterator)))
    dpg.set_value("INDEX_IMG",str(IDX+1))
    dpg.set_value("Image_Path",PATHS[IDX+1])
    render_image(PATHS[IDX+1])


def render_app():
    with dpg.item_handler_registry(tag="APP_HANDLER") as handler:
        dpg.add_item_clicked_handler(callback=app)
    #!!! BINDED IN ENGINE.APP