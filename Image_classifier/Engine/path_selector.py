import dearpygui.dearpygui as dpg
import Engine.__paths_sys
import Engine.global_variables as G

Paths_list = G.ACCEPTED

def __save_data():
    G.PATH = dpg.get_value("folder_path")
    G.OUTPUT = dpg.get_value("output_path")
    G.ACCEPTED = Paths_list
    G.MODE = dpg.get_item_label("Mode")

def __path_callback():
    path = Engine.__paths_sys.ask_folder_path()
    dpg.set_value(item="folder_path", value=path)
    __save_data()

def __output_callback():
    path = Engine.__paths_sys.ask_folder_path()
    dpg.set_value(item="output_path", value=path)
    __save_data()

def __ext_callback(sender,value):
    extension = str(value)
    if(len(value) == 0):
        return
    if(value[0] != '.'):
        extension = "." + extension.upper()
    Paths_list.append(extension)
    dpg.set_value("Input value","")
    dpg.focus_item("Input value")
    __update_ui()
    __save_data()

def __update_ui():
    if(dpg.does_alias_exist("BAB")):
        dpg.delete_item("BAB", children_only=True)
    if(dpg.does_item_exist("BAB")):
        for path in Paths_list:
            dpg.add_button(label=path, callback=__delete_path, parent="BAB", id=path)
        return
    with dpg.group(horizontal=True, parent="AA", id="BAB"):
        for path in Paths_list:
            dpg.add_button(label=path, callback=__delete_path, id=path)

def __delete_path(sender,value):
    print(sender)
    Paths_list.remove(sender)
    __update_ui()

def __change_mode(sender,value):
    mode = dpg.get_item_label("Mode")
    if( mode == "Copy"):
        dpg.set_item_label("Mode","Move")
    else:
        dpg.set_item_label("Mode","Copy")
        __save_data()

def __save_quit():
    return

def select_path():
    with dpg.window(label="PATH, and settings", id="AA"):
        dpg.add_text("Please select source path for classification")
        dpg.add_text("/Path_placeholder", tag="folder_path", color=(255,0,255))
        dpg.add_button(label="Choose folder path", callback=__path_callback)
        dpg.add_text("Please choose accepted image file types, click on a type to delete It")
        __update_ui()
        dpg.add_input_text(label="write extension and click Enter",tag="Input value",callback=__ext_callback, on_enter=True, width=100)
        dpg.add_text("Please select output directory")
        dpg.add_text("/Path_placeholder", tag="output_path", color=(255,0,255))
        dpg.add_button(label="Choose output path", callback=__output_callback)
        dpg.add_text("Please select mode, either 'Move' or 'Copy', mode move will change directory of Images, and copy will create copies of images")
        dpg.add_button(id="Mode", label="Copy", callback=__change_mode)
        dpg.add_text("If you are sure you've inserted all the data needed:", color=(150,255,50))
        dpg.add_button(width=150, height=50, label="Save and Continue", callback= lambda: __save_quit(), id="Save")

