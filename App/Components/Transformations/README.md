This folder collects Transformations of images. These transformations are divided to 2 files:
    -   Basic_Transofrmations.h - Collects a set of simple and multi-usable algorithms like dilation, erosion etc. 
    These algorithms are used many times, by preprocessing algorithms and watershed algorithms
    -   Transformations.h - Collects algorithms of Image preprocessing and opening. For this work it is:
        *   Illumination Gradient Normalization
        *   Folder Brightness Normalization
        *   Noise Reduction
        *   Resize
    !Transofrmations.h inheritates all functions of Basic_Transofrmations, so only Transformations.h is imported most of the times!

In Transformations.h present are 2 algorithms:
-- Noise removal
-- Illumination gradient removal
They are described in .PDF files in a form of science articles.
