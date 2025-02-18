"""
This file reads and saves the .option file for algorithms
It's structure is defined with "sample_options"
"""

class Algorithm_Option:
    """
    Class/Object "Algorithm_Option" stores and allows to manipulation of option from .option file for algorithm options
    """
    def __init__(self, row:str, option_class:str, row_idx:int) -> None:
        if type(row) != type("string"):
            raise Exception("Error, passed object for row in AlgorithmOption is not string")
        self.opt_index = row_idx
        self.opt_class = option_class
        self.initialized = True
        feedback = self._read_line(row)
        if(feedback == False):
            self.initialized = False
        pass
    
    def _read_line(self, row:str):
        """Private method for reading the row from .option file"""
        #Check if line is not truncated"
        if(row.find('<') == -1 or row.find('>') == -1):
            return False
        self.opt_title = row[0:row.find('<')].strip()
        self.opt_value_type = row[row.find('<')+1:row.find('>')]
        #Check if type is defined
        if(self.opt_value_type.upper()[0] != "D" and self.opt_value_type.upper()[0] != "I"):
            return False
        #Check if any number is defined
        if(len(row[row.find('>')+2:len(row)].strip()) <= 0):
            return False
        #Check if that is actually a number:
        try: 
            float(row[row.find('>')+2:len(row)].strip())
        except:
            #Not a number in that line
            return False
        if(self.opt_value_type.upper()[0] == "D"):
            self.opt_value = float(row[row.find('>')+2:len(row)].strip())
            return True
        self.opt_value = int(row[row.find('>')+2:len(row)].strip())
        return True
    
    def __str__(self):
        return str("Algorithm option: " + str(self.opt_title).upper() + ", value:<" + str(self.opt_value_type) + "> " +
                   str(self.opt_value).upper() + ", in class: " + str(self.opt_class).upper() + ", in order idx: " + str(self.opt_index))
        
    def is_matching_name(self, value: str):
        """Method checks if the name of the option is matching, not case sensitive"""
        return value.upper() == self.opt_title.upper()
    
    def is_matching_class(self, value:str):
        """Method checks if the class of the option is matching, not case sensitive"""
        return value.upper() == self.opt_class.upper()

    def is_matching_value(self, value: int|float):
        """Method checks if value of method is matching"""
        return value == self.opt_value
    
    def get_option(self):
        """Method returns the option as an object with 5 fields"""
        return {
            "NAME": self.opt_title,
            "VALUE": self.opt_value,
            "TYPE": self.opt_value_type,
            "IDX": self.opt_index,
            "CLASS": self.opt_class
        }
        
    def set_value(self, value: int|float):
        """Method sets new value to the option"""
        self.opt_value = value
        
    def create_line(self, class_name = False) -> str:
        """
        Function generates the string as would be in file in .option file
        If needed to return name of the class, argument "class_name" cna be passed as True
        """
        
        if(class_name == True):
            return str("-" + self.opt_class.upper())
        
        opt_str:str = "    "
        opt_str += str(self.opt_title) + "<" + str(self.opt_value_type) + ">: " + str(self.opt_value)
        return opt_str
        
        
        
        
    
    