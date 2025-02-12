import dearpygui.dearpygui as dpg

"""
File for handling options optimization, therefore here will be displaying of the options
And possibility of creating example mask and try to fit the options to best cover the mask
"""

class OptionsHandler:
    def __init__(self):
        self.setup_options()
        
        pass
    
    
    def setup_options(self):
        with dpg.group(parent="main"):
            dpg.add_text("Choose this option for optimization of options", color=(100,255,255))
            dpg.add_button(label="AutoFit options with mask", callback=self.autofit_options)
        
    def autofit_options(self):
        with dpg.window(label="Autofit configuration"):
            dpg.add_text("Please select path to folder which needs to be autofited, remember that all images from subdirectories are also included")
            with dpg.group(horizontal=True):
                dpg.add_text(source="Autofit_path", color=(100,250,100))
                dpg.add_button(label="Please select path to folder", callback=None)
            with dpg.group(horizontal=True):
                dpg.add_checkbox(label="Please check if needed is separate .option file for each subdirectory, or one .option file for all images in specified directory")