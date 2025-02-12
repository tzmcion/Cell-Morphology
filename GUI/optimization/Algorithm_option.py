"""
This file reads and saves the .option file for algorithms
It's structure is defined with "sample_options"
"""

class Algorithm_Option:
    """
    Field() -> Option_title
    Field() -> Option_value
    Field() -> Option_class #BACKGROUND_MASK, or FOREGROUND_REGIONS
    Field() -> Option_index #index by the order of options (as it was red)
    Field() -> Option_value_type #Either double or int
    """
    def __init__(self, row:str, option_class:str, row_idx:int) -> None:
        if type(row) != type("string"):
            raise Exception("Error, passed object for row in AlgorithmOption is not string")
        self.opt_index = row_idx
        self.opt_class = option_class
        self.read_line(row)
        pass
    
    def read_line(self, row:str):
        self.opt_title = row[0:row.find('<')].strip()
        self.opt_value_type = row[row.find('<')+1:row.find('>')]
        if(self.opt_value_type.upper()[0] == "D"):
            self.opt_value = float(row[row.find('>')+2:len(row)].strip())
            return
        self.opt_value = int(row[row.find('>')+2:len(row)].strip())
        pass
    
    def __str__(self):
        return str("Algorithm option: " + str(self.opt_title).upper() + ", value:<" + str(self.opt_value_type) + "> " +
                   str(self.opt_value).upper() + ", in class: " + str(self.opt_class).upper() + ", in order idx: " + str(self.opt_index))
        
    def is_matching_name(self, value: str):
        return value.upper() == self.opt_title.upper()
    
    def is_matching_class(self, value:str):
        return value == self.opt_class

    def is_matching_value(self, value: int|float):
        return value == self.opt_value
    
    def get_option(self):
        return {
            "NAME": self.opt_title,
            "VALUE": self.opt_value,
            "TYPE": self.opt_value_type,
            "IDX": self.opt_index,
            "CLASS": self.opt_class
        }
        
    def set_value(self, value: int|float):
        self.opt_value = value
        
    def create_line(self, class_name = False) -> str:
        """
        Function generates the string as would be in file in .option file
        """
        
        if(class_name == True):
            return str("-" + self.opt_class.upper())
        
        opt_str:str = "    "
        opt_str += str(self.opt_title) + "<" + str(self.opt_value_type) + ">: " + str(self.opt_value)
        return opt_str
        
        
        
        
    
    