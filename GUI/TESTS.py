"""
File for testing of options file
"""

from optimization.OptionsReader import OptionsReader

def test_reader_options():
    FILE_PATH = "/home/tymoteuszapriasz/KM/GUI/optimization/sample_options.option"
    reader = OptionsReader(FILE_PATH)
    reader.change_option_by_name(25,"dil_er_size")
    assert reader.get_option_by_name("dil_er_size", "FOREGROUND_MASK").opt_value == 25
    pass

def test_reader_save():
    FILE_PATH = "/home/tymoteuszapriasz/KM/GUI/optimization/sample_options.option"
    reader = OptionsReader(FILE_PATH)
    reader.change_option_by_name(25,"dil_er_size")
    reader.save_options("./test_save.option")
    reader = OptionsReader("./test_save.option")
    assert reader.get_option_by_name("dil_er_size", "FOREGROUND_MASK").opt_value == 25
