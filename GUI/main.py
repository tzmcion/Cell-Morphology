"""
Here will be Graphical User Interface
Handled with DearPyGui, which is (I hope) a good idea
GUI will handle all the executions, and optimization of the algorithms.
Especially, It will display all the options.
"""
import dearpygui.dearpygui as dpg
import config
from Display.display_options import OptionsHandler

#Define global Variables
VERSION = "0.2"

def __main__():
    dpg.create_context()
    dpg.create_viewport(decorated=True, title="Cell classifier and detector system",width=1440, height=880)
    dpg.setup_dearpygui()
    
    #Create main registry
    with dpg.value_registry():
        dpg.add_string_value(default_value="PATH: Undefined", tag="Images_path")
        dpg.add_string_value(default_value="PATH: Undefined",tag="Autofit_path")
    
    #Create the main window
    with dpg.window(label="Main window",no_close=True,no_collapse=True,id="main",width=1440,height=880):
        dpg.add_text("Welcome to cell classifier v" + str(VERSION) + ". Please one or more of the following options")
        
        
    #Here load other components/modules
    opt = OptionsHandler()
    ###
    dpg.show_viewport()
    # dpg.start_dearpygui()
    while dpg.is_dearpygui_running():
        #Main Loop
        #Literally anything can go here
        #Halleluja!
        for function in config.frame_functions:
            function()
        dpg.render_dearpygui_frame()
    dpg.destroy_context()
    

#Run the main function
__main__()
    