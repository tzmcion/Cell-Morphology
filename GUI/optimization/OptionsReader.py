"""
This file reads and saves the .option file for algorithms
It's structure is defined with "sample_options"
"""

from optimization.Algorithm_option import Algorithm_Option

class OptionsReader():
    """
    Class for reading options from the file, altering the oprions, and saving them to the new file
    """
    
    def __init__(self, opt_path) -> None:
        self.opts_array:list[Algorithm_Option] = []
        self.iterate_idx = 0
        self.initialized = [True,-1]
        current_class = ""
        idx = 0
        with open(opt_path,"r") as file:
            for line in file:
                line_raw = self._delete_comments(line).strip()
                if(len(line_raw) <= 0):
                    continue
                if(line_raw[0] == "-"):
                    current_class = line_raw[1:len(line_raw)]
                    continue
                if(current_class == ""):
                    #File truncated (incorrect format - no class specified before option)
                    self.initialized = [False, idx]
                    break
                self.opts_array.append(Algorithm_Option(line_raw,current_class,idx))
                if(self.opts_array[len(self.opts_array)-1].initialized == False):
                    #File truncated (incorrect in format - incorrect line format)
                    self.initialized = [False, idx]
                    break
                idx+=1
            file.close()
            
    def __str__(self):
        ret = str("Options object containing " + str(len(self.opts_array)) + " options \n")
        for opt in self.opts_array:
            ret += str("[ " + str(opt) + " ]\n")
        return ret
        
    def _delete_comments(self, line:str) -> str:
        return line[0:line.find('#')]   #much simpler than in c++...
    
    def iterate_options(self, reset = False)-> Algorithm_Option | None:
        """Method is an iterator, returns next option, pass argument False to reset the index from zero"""
        if(reset):
            self.iterate_idx = 0
        if(self.iterate_idx >= len(self.opts_array)):
            return None
        self.iterate_idx+=1
        return self.opts_array[self.iterate_idx-1]
    
    def change_option_by_idx(self, value: float|int, idx:int)-> None:
        """Method changes the value of function by index"""
        if(idx >= len(self.opts_array)):
            raise Exception(" Provided value for index is out of range: " + str(idx))
        self.opts_array[idx].set_value(value)
        return
    
    def change_option_by_name(self, value: float|int, name:str, class_name:str) -> None:
        """Method changes the option bi it's name and class_name (algorithm name)"""
        for option in self.opts_array:
            if option.is_matching_name(name) and option.is_matching_class(class_name):
                option.set_value(value)
                return
        raise Exception ("Name not found in options: " + name)
    
    def get_option_by_name(self,name:str, class_name:str) -> Algorithm_Option:
        """Method provides the option by it's name and class name"""
        for option in self.opts_array:
            if option.is_matching_name(name) and option.is_matching_class(class_name):
                return option
        return None
    
    def save_options(self, path_to_save:str, header_comment="FILE GENERATED AUTOMATICALLY") -> None:
        """Method saves the file to """
        class_name = ""
        with open(path_to_save,"w") as file:
            file.write("#" + str(header_comment) + "\n")
            file.write("# AVNT OPTIMIZER @2025 \n \n")
            for opt in self.opts_array:
                if(opt.opt_class != class_name):
                    file.write(opt.create_line(True))
                    file.write('\n')
                    class_name = opt.opt_class
                file.write(opt.create_line())
                file.write('\n')
            file.close()
        pass