"""
This file handles the user mask drawing
"""
from optimization.OptionsReader import OptionsReader
from optimization.ModuleIntegration import ModuleIntegration
from Display.path_selector import save_paths_to_file
from Display.results_display import resultsDisplay
import dearpygui.dearpygui as dpg
import os
import numpy as np
import cv2

class userDraw:
    
    def __init__(self, parent:str, temp_folder:str, options_ref:OptionsReader, images:list[str]):
        print(temp_folder)
        #Axis mask is structured like opencv connected components, so filled with zeros, and fields assigned to object is either 1,2,3 etc...
        self.parent = parent #parent for dpg to texture display
        self.draw_axis:np.array = [] #This will be matrix of pixel positions of the mask.
        self.temp_path:str = temp_folder #Path to folder where images and masks can be stored, overdrived and deleted
        self.options:OptionsReader = options_ref #Reference to the options, which will be updated frequently
        self.images:list[str] = images #List of paths to affected images
        self.mouse_down = False
        self.object_id = 1
        self.last_positions = [0.0,0.0]
        save_paths_to_file(self.images, self.temp_path + "/images_path_list.txt")
        options_ref.save_options(self.temp_path + "/options_tmp.option")
        with dpg.group(horizontal=True, parent=parent):
            dpg.add_button(label="Draw/Get different sample", callback=self.get_diff_sample)
            dpg.add_button(label="Save mask and start", callback=self.start_optimization)
            dpg.add_button(label="Create order of options (for dev)", callback=self.start_ordering)
        with dpg.drawlist(600,600,label="Drawing canvas", id="Image_Canvas",pos=(150,150), parent=parent):
            dpg.add_draw_layer(tag="Image_Canvas_Layer_one")
        dpg.add_button(parent=parent, label="Next Object (or click space)", callback=None)
        dpg.add_button(parent=parent, label="Delete last operation (or click \"Z\")", callback=None)
        self._start_subprocess()
        
    def _start_subprocess(self):
        #First, delete the info file
        if(os.path.exists(self.temp_path + "/info.txt")):
            os.remove(self.temp_path + "/info.txt")
        if(os.path.exists(self.temp_path + "/cropped_bg.jpg")):
            os.remove(self.temp_path + "/cropped_bg.jpg")        
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
        self.array_of_states = [self.draw_axis.copy()]
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
        if(dpg.does_item_exist("Image_Canvas")):
            for field in fields_to_colour:
                dpg.draw_rectangle((field[1],field[0]),(field[1] + size, field[0] + size),color=(int(155 / object_id * 50) % 255,object_id*55 % 255,255,10), fill=(int(155 / object_id * 50) % 255,object_id*55 % 255,255,10),parent="Image_Canvas")
                self.draw_axis[field[0],field[1]] = object_id
    
    def helper_draw_line(self,x, y, d_x, d_y, object_id):
            x2, y2 = x + d_x, y + d_y
            # Bresenham's line algorithm
            dx = abs(x2 - x)
            dy = abs(y2 - y)
            sx = 1 if x < x2 else -1
            sy = 1 if y < y2 else -1
            err = dx - dy
            
            matriz = self.draw_axis
            
            while True:
                if 0 <= x < matriz.shape[1] and 0 <= y < matriz.shape[0]:
                    matriz[y, x] = object_id  # Assign value 4 to the pixel
                
                if x == x2 and y == y2:
                    break
                
                e2 = 2 * err
                if e2 > -dy:
                    err -= dy
                    x += sx
                if e2 < dx:
                    err += dx
                    y += sy
            
            self.draw_axis = matriz
    
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
                self.last_positions = [-1,-1]
                self.array_of_states.append(self.draw_axis.copy())
                return
            if(str(point_data).find(',') == -1):
                return
            x_pos = float(str(point_data).split(",")[0].strip()[1:])
            y_pos = float(str(point_data).split(",")[1].strip()[:-1])
            if(sender=="mouse_move"):
                last_x, last_y = self.last_positions
                last_x -= cip[0]
                last_y -= cip[1]
                self.last_positions = [x_pos,y_pos]
                if(self.mouse_down == False):
                    return
                if(cip[0] < x_pos and cip[0] + W > x_pos and cip[1] < y_pos and cip[1] + H > y_pos):
                    actual_x = int(x_pos - cip[0])
                    actual_y = int(y_pos - cip[1])
                    width = height = 600
                    if(actual_x >= width):
                        actual_x = 595
                    if(actual_y >= height):
                        actual_y = 595
                    if(actual_x < 0):
                        actual_x = 0
                    if(actual_y < 0):
                        actual_y = 0
                    
                    #Here, draw a line between last_x,last_y
                    if(last_x != -1):
                        self.helper_draw_line(actual_x,actual_y,int(last_x - actual_x),int(last_y - actual_y),object_id)
                    size = 3
                    dpg.draw_rectangle((actual_x,actual_y),(actual_x + size, actual_y + size),color=(155 / object_id * 50 % 25,object_id*25 % 255,255,20), fill=(155 / object_id * 50 % 25,object_id*25 % 255,255,20),parent="Image_Canvas")
                    for x in range (0,size):
                        for y in range(0,size):
                            dr_x = actual_y + x
                            dr_y = actual_x + y
                            if(dr_x >= 600):
                                dr_x = 595
                            if(dr_y >= 600):
                                dr_y = 595
                            self.draw_axis[dr_x,dr_y] = object_id #!!! FIRST COLLUMN
                            
        def handle_keyboard(data,sender):
            #524 = space // which is creating new object
            #571 = z //which is going back
            if(sender == 524):
                self.object_id += 1
                return
            if(sender == 571):
                if(np.array_equal(self.draw_axis,self.array_of_states)):
                    self.array_of_states.pop()
                if(len(self.array_of_states) > 0):
                    self.draw_axis = self.array_of_states.pop()
                else:
                    height = width = 600
                    self.draw_axis = np.zeros((height, width), dtype=np.uint8)
                dpg.delete_item("Image_Canvas",children_only=True)
                #Now, rerender the new one
                self.module_integrator.read_from_file_and_display(self.temp_path + "/cropped_bg.jpg","Image_Canvas")
                size = 2
                for y in range (0, self.draw_axis.shape[0]):
                    for x in range (0, self.draw_axis.shape[1]):
                        if self.draw_axis[y,x] != 0:
                            val = self.draw_axis[y,x]
                            dpg.draw_rectangle((x,y),(x + size,y + size), color=(int(155 / val * 50) % 255, val*55 % 255,255,10), fill=(int(155 / val * 50) % 255, val*55 % 255,255,10),parent="Image_Canvas")
                #rewrite the whole data and remove last operation
            
        
        with dpg.handler_registry():
            #registry to get that user pressed down on image
            dpg.add_mouse_down_handler(tag="mouse_down", callback=handle_handlers)
            #registry to get that user moves a mouse (with setup flag is_pressed_down)
            dpg.add_mouse_move_handler(tag="mouse_move", callback=handle_handlers)
            #and lastly, registry to handle mouse up
            dpg.add_mouse_release_handler(tag="mouse_up", callback=handle_handlers)
            #And add keyboard keys
            dpg.add_key_press_handler(tag="key_press", callback=handle_keyboard)
            
    def get_diff_sample(self):
        self.module_integrator.write_to_file(self.temp_path + "/info.txt","!NEW_SAMPLE")
        if(os.path.exists(self.temp_path + "/cropped_bg.jpg")):
            os.remove(self.temp_path + "/cropped_bg.jpg")
        self.module_integrator.await_file_change(self.temp_path + "/info.txt")
        dpg.delete_item("Image_Canvas",children_only=True)
        width = height = 600
        self.draw_axis = np.zeros((height, width), dtype=np.uint8) 
        self.module_integrator.await_file_create(self.temp_path + "/cropped_bg.jpg")
        self.module_integrator.read_from_file_and_display(self.temp_path + "/cropped_bg.jpg","Image_Canvas")
        pass
    
    def start_optimization(self):
        #Save current mask and proceed to new window/module
        cv2.imwrite(self.temp_path + "/generated_mask.bmp",self.draw_axis*10)
        self.module_integrator.write_to_file(self.temp_path + "/info.txt", "!START")
        self.results = resultsDisplay("Image_Canvas",self.options, self.temp_path)
        pass
    
    def start_ordering(self):
        cv2.imwrite(self.temp_path + "/generated_mask.bmp",self.draw_axis*10)
        self.module_integrator.write_to_file(self.temp_path + "/info.txt", "!ORDER")
        self.results = resultsDisplay("Image_Canvas",self.options, self.temp_path)
        pass
        
    