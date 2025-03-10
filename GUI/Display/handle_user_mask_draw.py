"""
This file handles the user mask drawing
"""
from optimization.OptionsReader import OptionsReader
from optimization.ModuleIntegration import ModuleIntegration
from Display.path_selector import save_paths_to_file
import dearpygui.dearpygui as dpg
import os
import numpy as np

class userDraw:
    
    def __init__(self, parent:str, temp_folder:str, options_ref:OptionsReader, images:list[str]):
        print(temp_folder)
        #Axis mask is structured like opencv connected components, so filled with zeros, and fields assigned to object is either 1,2,3 etc...
        self.parent = parent #parent for dpg to texture display
        self.draw_axis:list[list] = [] #This will be matrix of pixel positions of the mask.
        self.temp_path:str = temp_folder #Path to folder where images and masks can be stored, overdrived and deleted
        self.options:OptionsReader = options_ref #Reference to the options, which will be updated frequently
        self.images:list[str] = images #List of paths to affected images
        self.mouse_down = False
        self.object_id = 1
        self.last_positions = [0.0,0.0]
        save_paths_to_file(self.images, self.temp_path + "/images_path_list.txt")
        options_ref.save_options(self.temp_path + "/options_tmp.option")
        with dpg.drawlist(600,600,label="Drawing canvas", id="Image_Canvas",pos=(150,150), parent=parent):
            dpg.add_draw_layer(tag="Image_Canvas_Layer_one")
            dpg.add_draw_layer(tag="Masks")
        dpg.add_button(parent=parent, label="Next Object (or click space)", callback=None)
        dpg.add_button(parent=parent, label="Delete last operation (or click \"Z\")", callback=None)
        self._start_subprocess()
        
    def _start_subprocess(self):
        #First, delete the info file
        if(os.path.exists(self.temp_path + "/info.txt")):
            os.remove(self.temp_path + "/info.txt")
        #then start the integration part
        self.module_integrator = MI = ModuleIntegration()
        MI.run_subprocess([str(self.temp_path + "/options_tmp.option"),str(self.temp_path + "/images_path_list.txt"),self.temp_path])
        #Now_await file change, file will be
        MI.await_file_change(self.temp_path + "/info.txt")
        MI.read_from_file_and_display(self.temp_path + "/cropped_bg.jpg","Image_Canvas")
        #When file is ready, needed is to implement drawing
        #And set the matrix
        width = height = 600
        self.draw_axis = np.zeros((height, width), dtype=np.uint8)   #Initially, everything is counted as background
        self.mouse_movement_setup()
        pass
    
    def fill_the_space(self, object_id:int):
        """This function will fill the void space after drawing contours of an object"""
        row_in_between = []
        fields_row = []
        fields_col = []
        start = False
        index_c = index_r = 0
        for column in self.draw_axis:
            for row in column:
                if row == object_id and start == True:
                    for r in row_in_between:
                        fields_row.append(r)
                        row_in_between = []
                if(row) == object_id:
                    start = True
                if start:
                    row_in_between.append((index_c,index_r))
                index_r += 1
            index_c += 1
            index_r = 0
            row_in_between = []
            start = False
        for r in range (0,len(self.draw_axis)):
            for c in range (0,len(self.draw_axis)):
                if (self.draw_axis[c,r] == object_id and start == True):
                    for a in row_in_between:
                        fields_col.append(a)
                        row_in_between = []
                if self.draw_axis[c,r] == object_id:
                    start = True
                if start:
                    row_in_between.append((c,r))
            row_in_between = []
            start = False
        #And now rewrite the matrix and draw it
        fields_row = set(fields_row)
        fields_col = set(fields_col)
        fields_to_colour = fields_col.intersection(fields_row)
        size = 3
        for field in fields_to_colour:
             dpg.draw_rectangle((field[1],field[0]),(field[1] + size, field[0] + size),color=(int(155 / object_id * 50) % 255,object_id*55 % 255,255,10), fill=(int(155 / object_id * 50) % 255,object_id*55 % 255,255,10),parent="Image_Canvas")
             self.draw_axis[field[1],field[0]] = object_id
        print(fields_to_colour)
    
    def mouse_movement_setup(self):
        #define function to handle the registries
        def handle_handlers(sender, point_data):
            W = H = 600 #dimensions of image
            #The dimensions will be fixed
            #X = 33, Y = 120
            object_id = self.object_id
            cip = [33.0,120.0]
            if(sender=="mouse_down"):
                x_pos, y_pos = self.last_positions
                if(cip[0] < x_pos and cip[0] + W > x_pos and cip[1] < y_pos and cip[1] + H > y_pos):
                    self.mouse_down = True
                return
            if(sender=="mouse_up"):
                self.fill_the_space(object_id)
                self.mouse_down = False
                self.object_id += 1
                return
            if(str(point_data).find(',') == -1):
                return
            x_pos = float(str(point_data).split(",")[0].strip()[1:])
            y_pos = float(str(point_data).split(",")[1].strip()[:-1])
            if(sender=="mouse_move"):
                self.last_positions = [x_pos,y_pos]
                if(self.mouse_down == False):
                    return
                if(cip[0] < x_pos and cip[0] + W > x_pos and cip[1] < y_pos and cip[1] + H > y_pos):
                    actual_x = int(x_pos - cip[0])
                    actual_y = int(y_pos - cip[1])
                    size = 8
                    dpg.draw_rectangle((actual_x,actual_y),(actual_x + size, actual_y + size),color=(155 / object_id * 50 % 25,object_id*25 % 255,255,20), fill=(155 / object_id * 50 % 25,object_id*25 % 255,255,20),parent="Image_Canvas")
                    for x in range (0,size):
                        for y in range(0,size):
                            self.draw_axis[actual_y + x,actual_x + y] = object_id #!!! FIRST COLLUMN
        
        with dpg.handler_registry():
            #registry to get that user pressed down on image
            dpg.add_mouse_down_handler(tag="mouse_down", callback=handle_handlers)
            #registry to get that user moves a mouse (with setup flag is_pressed_down)
            dpg.add_mouse_move_handler(tag="mouse_move", callback=handle_handlers)
            #and lastly, registry to handle mouse up
            dpg.add_mouse_release_handler(tag="mouse_up", callback=handle_handlers)
        
    