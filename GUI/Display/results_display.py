import dearpygui.dearpygui as dpg
import config
from optimization.OptionsReader import OptionsReader


class resultsDisplay:
    
    def __init__(self, image_parent:str, options:OptionsReader):
        self.image_cnavas = image_parent
        self.options = options
        
        def caller():
            print("buja")
            
        config.frame_functions.append(caller)
        pass
    
    def read_and_display_options(self):
        
        pass