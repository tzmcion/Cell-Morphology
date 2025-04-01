import dearpygui.dearpygui as dpg
from optimization.Algorithm_option import Algorithm_Option
from optimization.OptionsReader import OptionsReader
from Display.path_selector import ask_folder_path, study_path, get_all_images
from Display.handle_user_mask_draw import userDraw
import os
import glob

"""
File for handling options optimization, therefore here will be displaying of the options
And possibility of creating example mask and try to fit the options to best cover the mask
"""

class OptionsHandler:
    def __init__(self):
        self.setup_options()
        self.reader = None
        self.integrator = None
        #Cleanup temp folder
        for file in glob.glob(os.path.join(str(os.getcwd() + "/GUI/temp"), "*")):
            os.remove(file)
        pass
    
    
    def setup_options(self):
        with dpg.group(parent="main"):
            dpg.add_text("Choose this option for optimization of options", color=(100,255,255))
            dpg.add_button(label="AutoFit options with mask", callback=self.autofit_options)
        with dpg.value_registry():
            dpg.add_string_value(default_value="None",tag="DIR_PATH")
            
    def set_folder_path(self):
        DIRECTORY = ask_folder_path()
        dpg.set_value("Autofit_path",DIRECTORY)
        self.add_window_with_options()
        
    def autofit_options(self):
        with dpg.window(label="Autofit configuration"):
            dpg.add_text("Please select path to folder which needs to be autofited, remember that all images from subdirectories are also included")
            with dpg.group(horizontal=True):
                dpg.add_text(source="Autofit_path", color=(100,250,100))
                dpg.add_button(label="Please select path to folder", callback=self.set_folder_path)
            # dpg.add_checkbox(label="Please check if needed is separate .option file for each subdirectory, or one .option file for all images in specified directory")
                
    def change_option_value(self,arg:str,new_val:str):
        [class_name, option_name] = arg.split('~')
        input_type = self.reader.get_option_by_name(option_name,class_name).opt_value_type
        try:
            if(input_type == "int"):
                int(new_val)
            if(input_type == "double"):
                float(new_val)
        except:
            print("Wrong value")
            return
        if(input_type == "int"):
            new_val = int(new_val)
        else:
            new_val = float(new_val)
        #Value updated
        self.reader.change_option_by_name(new_val,option_name,class_name)
        
        
    def display_single_option(self,parent:str, option:Algorithm_Option) -> None:
        with dpg.group(parent=parent, horizontal=True):
            opt_val = option.opt_title
            for a in range(0,30 - len(option.opt_title)):
                opt_val += " "
            dpg.add_text(default_value=opt_val)
            dpg.add_input_text(tag=str(option.opt_class+"~"+option.opt_title),default_value=str(option.opt_value),width=150,callback=self.change_option_value)     
        
    def start_optimalziation(self):
        #need to collect all the options
        #then user will be prompted to draw the goal boundaries
        #Options_Optimizer from App/Utils is ready to recieve a mask
        dpg.set_item_pos("options_display",(1440-600,0))
        with dpg.window(label="Get Necessary data",pos=(25,25),width=1440-650,height=900,id="mask_drawer", no_move=True, no_resize=True):
            with dpg.group(horizontal=True):
                dpg.add_text("Please insert the number of iterations")
                dpg.add_input_int(default_value=1000, width=200)
            with dpg.group(horizontal=True):
                dpg.add_text("Please insert the number of threads which you wish to use")
                dpg.add_input_int(default_value=2, width=200)
        all_paths = get_all_images(dpg.get_value("Autofit_path"))
        self.integrator = userDraw("mask_drawer",str(os.getcwd() + "/GUI/temp"),self.reader,all_paths)
        
    
    
    def add_window_with_options(self):
        DIR_DATA = study_path(dpg.get_value("Autofit_path"))
        path_options = "GUI/optimization/sample_options.option"
        with dpg.window(width=600,height=850,tag="options_display",label="Algorithm parameters"):
            dpg.add_text(str("Detected " + str(len(DIR_DATA[0])) + " directories and " + str(len(DIR_DATA[1])) + " images in selected directory"))
            print(DIR_DATA[2])
            if(len(DIR_DATA[2]) == 0):
                dpg.add_text("No .option file detected in selected directory, using default file")
            else:
                path_options = DIR_DATA[2][0]
            dpg.add_text("Here you can edit the starting parameters of optimalization")
            dpg.add_button(label="START OPTIMALIZATION", callback=self.start_optimalziation)
        self.reader = OptionsReader(path_options)
        last_class = ""
        for option in self.reader.opts_array:
            if(last_class != option.opt_class):
                dpg.add_text(parent="options_display",color=(150,255,255),default_value=option.opt_class)
                last_class = option.opt_class
            self.display_single_option("options_display",option)
    
    