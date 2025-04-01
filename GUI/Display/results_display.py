import dearpygui.dearpygui as dpg
import config
import functools
import time
import os
import shutil
from optimization.OptionsReader import OptionsReader
from optimization.Algorithm_option import Algorithm_Option
from Display.path_selector import ask_save_path
from optimization.ModuleIntegration import ModuleIntegration


class resultsDisplay:
    
    def __init__(self, image_parent:str, options:OptionsReader, temp_folder:str):
        self.image_cnavas = image_parent
        self.options: OptionsReader = options
        self.temp_folder = temp_folder
        self.time = time.time()
            
            
        config.frame_functions.append(functools.partial(self.caller))
        self.options.opts_array = []
        self.finished()
        pass
    
    def change_single_option(self,value, option:Algorithm_Option) -> None:
        tag = str(option.opt_class+"~"+option.opt_title)
        dpg.set_value(tag, value)
        
    def caller(self):
        if(int(time.time()) % 5 == 0):
            self.time = time.time()
            self.read_and_display_options()
        pass
        
    def display_single_option(self,parent:str, option:Algorithm_Option) -> None:
        with dpg.group(parent=parent, horizontal=True):
            opt_val = option.opt_title
            for a in range(0,30 - len(option.opt_title)):
                opt_val += " "
            dpg.add_text(default_value=opt_val)
            dpg.add_text(tag=str(option.opt_class+"~"+option.opt_title),default_value=str(option.opt_value))     
    
    def draw_options(self, parent, options):
        dpg.delete_item(parent, children_only=True)
        last_class = ""
        for option in options.opts_array:
            if(last_class != option.opt_class):
                dpg.add_text(parent=parent,color=(150,255,255),default_value=option.opt_class)
                last_class = option.opt_class
            self.display_single_option(parent,option)
    
    def read_and_display_options(self):
        if not (os.path.exists(self.temp_folder + "/options_out.option")):
            return
        options = OptionsReader(self.temp_folder + "/options_out.option")
        for option in options.opts_array:
            tag = option.opt_class+"~"+option.opt_title
            value = dpg.get_value(tag)
            if(value != option.opt_value):
                self.draw_options("options_display", options)
        pass
    
    def save_results(self):
        if not (os.path.exists(self.temp_folder + "/options_out.option")):
            return
        path = ask_save_path()
        shutil.copyfile(self.temp_folder + "/options_out.option", path)
    
    def finished(self):
        mo = ModuleIntegration()
        mo.await_file_change(self.temp_folder + "./info.txt")
        #File can only change to done (currently when writing this :P )
        config.frame_functions.clear()
        #Delete the currently displayed windows, "options_display" and "mask_drawer"
        dpg.delete_item("mask_drawer")
        dpg.delete_item("options_display")
        with dpg.window(label="Optimization Results", id="results", width=800, height=200):
            dpg.add_text("Optimization Finished", color=(125,255,125))
            dpg.add_text("Click the button to save the file")
            dpg.add_button(label="Save", callback=self.save_results)
        
        