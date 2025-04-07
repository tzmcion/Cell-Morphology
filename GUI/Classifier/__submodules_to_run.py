"""Modules which are to run and their arguments"""

class Submodules:
    @staticmethod
    def Cell_Counter():
        """
        Algorithm counts cells on the image by using halo filtering to detect cell centers
        The result of the algorithm is a sum of cells on each patch provided
        Output: Images with dots in each detected as single cell, .csv file with numerical data
        """
        pass
    
    @staticmethod
    def Surface_Counter():
        """
        Algorithm sums the surface which is occupied by cells. It uses the concept of
        sure background to extract the regions evaluated as such.
        Output: Images with marked areas and .csv file with numerical data
        """
        pass
    
    @staticmethod
    def Full_Segmentation():
        """
        Segmentation with classification implemented via ResNet-50 model retrained with transfer learning.
        Algorithm uses Watershed to segment images, then validate each sample through method integrated via python module
        Output: Images with marked cells by classes, and .csv file with numerical data
        """
        pass
    
    @staticmethod
    def Normalization_brightness():
        """
        Module aims to delete and smoothen the illumination gradient which occures on the images
        Output: Images with illumination gradient reduced
        """
        pass
    
    @staticmethod
    def Normalization_global_brightness():
        """
        Module aims to normalize brightness in all of the samples, reduce the differences between brightness on different patches
        Output: Images with normalized brightness
        """
        pass
    
    @staticmethod
    def Normalization_noise():
        """
        Module aims to reduce the noise on the images, while preserving as many cell texture details as possible
        Output: Images with reduced noise and less "black dots"
        """
        pass
    
    @staticmethod
    def Normalization_size():
        """
        Module can cut and trim images into set size, preferrably squares
        Output: Images with set size and square size
        """
        pass
    
    @staticmethod
    def Options_optimizer():
        """
        Module aims to optimize options on provided images.
        Module requires images to be preprocessed (with Noise reduced, no illumination gradient, and with same brightness)
        Input: .option file with algorithm options, images
        Output: .option file with optimized Algorithm options
        """
        pass
    
    @staticmethod
    def Preprocess_all():
        """
        Module reads the .option file for simple algorithm running via c++.
        It allows the submodules to run on multiple threads, which is not possible with Python GUI,
        therefore accelerating the processing (mainly used for preprocessing)
        Input: .option file with Images
        Output: Images divided into folders
        """
        pass
    
    @staticmethod
    def Track_movement():
        """
        Module aims to track how the cels move in the image
        Work is still in progress here
        """
        pass
    
    

