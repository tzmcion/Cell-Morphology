import os
import subprocess
import time
import dearpygui.dearpygui as dpg
from optimization.OptionsReader import OptionsReader

class ModuleIntegration:
    
    def __init__(self):
        self.process = None
        self.module_path = "./App/Utils/Options_Optimizer"
        self.path_back = "../../../"
    
    def kill_subprocess(self):
        """Method kills the subprocess of optimalization"""
        self.process.terminate()
    
    def run_subprocess(self, arguments:list[str]) -> None:
        """Method runs the defined subprocess with specified arguments list"""
        ###Assuming that os.getcwd() is from the root of the application, so there is a folder called GUI,App etc.
        
        os.chdir("./App/Utils")
        self.process = subprocess.Popen(["./Options_Optimizer/main.out", arguments[0], arguments[1], arguments[2]])
        #Return back
        os.chdir("../../")
    
    def await_file_change(self,path:str, max_iterations = 1000) -> bool:
        """Method waits for file to change, """
        is_done = False
        last_state = ""
        if(os.path.exists(path)):
            with open(path,'r') as f:
                last_state = f.read()
        while is_done == False:
            max_iterations -= 1
            if(os.path.exists(path)):
                with open(path,'r') as f:
                    if(last_state != f.read):
                        is_done = True
                        break
            if(max_iterations < 0):
                break
            time.sleep(0.02) 
        if(is_done):
            return True
        return False
        pass
    
    def read_from_file_and_display(self, path:str, tag:str) -> None:
        """Function reads from the image and displays into specified tag of dpg"""
        if not os.path.exists(path):
            return
        if not dpg.does_item_exist(tag):
            return
        if dpg.does_item_exist("subprocess_textures"):
            dpg.delete_item("subprocess_textures")
        ###
        width, height, channels, data = dpg.load_image(path)
        with dpg.texture_registry(id="subprocess_textures"):
            dpg.add_static_texture(width,height,data,tag="sp_image")
        dpg.draw_image("sp_image",(0,0),(600, 600), uv_min=(0, 0), uv_max=(1, 1),parent=tag)
        
    def save_current_options(self, reader:OptionsReader) -> None:
        """Method saves current options in integrated module path"""
        reader.save_options(str(self.module_path + "/run_temp/alg_options.option"), "PROGRAM FILE DO NOT ALTER")
        
    def read_new_options(self, reader:OptionsReader) -> None:
        """Method reads the options from the integrated module"""
        reader.hard_read_options(str(self.module_path + "/run_temp/alg_options.option"))