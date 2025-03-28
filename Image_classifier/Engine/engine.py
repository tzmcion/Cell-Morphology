import dearpygui.dearpygui as dpg
import Engine.path_selector
import Engine.display_paths
import Engine.app

def main():
    dpg.create_context()
    dpg.create_viewport(title='Image Classifier', width=1000, height=1000)
    #
    Engine.path_selector.select_path()
    Engine.display_paths.all_paths()
    Engine.app.render_app()
    #
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()