"""
File for testing of options file
"""

from optimization.OptionsReader import OptionsReader

#This test checks if value of option was sucessfully changed
def test_reader_options():
    FILE_PATH = "GUI/optimization/sample_options.option"
    reader = OptionsReader(FILE_PATH)
    reader.change_option_by_name(25,"dil_er_size", "FOREGROUND_MASK")
    assert reader.get_option_by_name("dil_er_size", "FOREGROUND_MASK").opt_value == 25
    pass

#This test reads the sample .option file, chooses one option to replace, then saves the file and reads it again, checking if option was saved
def test_reader_save():
    FILE_PATH = "GUI/optimization/sample_options.option"
    reader = OptionsReader(FILE_PATH)
    reader.change_option_by_name(15,"dil_er_size", "FOREGROUND_MASK")
    reader.save_options("./test_save.option")
    reader = OptionsReader("./test_save.option")
    assert reader.get_option_by_name("dil_er_size", "FOREGROUND_MASK").opt_value == 15
    
#Test checks if options_reader can correctly display which row of .option file is incorrect
def test_truncated_read():
    FILE_PATH = "GUI/TESTS/FILE_SAMPLES/truncated_algorithm_options.option"
    FILE_PATH_2 = "GUI/TESTS/FILE_SAMPLES/truncated_algorithm_options_2.option"
    reader = OptionsReader(FILE_PATH)
    assert reader.initialized[0] == False
    assert reader.initialized[1] == 1
    reader = OptionsReader(FILE_PATH_2)
    assert reader.initialized[0] == False
    assert reader.initialized[1] == 5
