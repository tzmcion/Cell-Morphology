import dearpygui.dearpygui as dpg

class Menu:
    def __init__(self, parent:str):
        #Create the window with button
        with dpg.value_registry():
            dpg.add_bool_value(default_value=False, tag="counting_cells")
            dpg.add_bool_value(default_value=False, tag="surface_occupied")
            dpg.add_bool_value(default_value=False, tag="ratio_of_cells")
            dpg.add_bool_value(default_value=False, tag="cell_movement")
        
        with dpg.window(parent=parent,label="Main"):
            dpg.add_text("Main Analyzis", color=(0,255,255))
            dpg.add_checkbox(label="Include Total counting of cells", source="counting_cells")
            dpg.add_checkbox(label="Include Total surface occupied by cells", source="surface_occupied")
            dpg.add_checkbox(label="Include Ratio of cell types and cell types counter", source="ratio_of_cells")
            dpg.add_checkbox(label="Include cell movement detection (soon)", source="cell_movement")
            dpg.add_button(label="Start", callback=None)
        pass
    
    def run_program(self):
        pass