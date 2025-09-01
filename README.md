<div align='center'>
  <img display='inline' src="https://github.com/user-attachments/assets/956e4f21-71b4-4e55-8f66-19828d040faa" width="100" alt="Logo">
  <h3 display='inline'>Morphology App</h3>
  <p>Segmentation and Classification of cells from Optical Microscope Images 🔬</p>
  <h5>*This repository is a part of science article <a href="https://link.springer.com/chapter/10.1007/978-3-031-99474-6_5">Classification of Skin Cell Structure in Optical Microscope Images Using Transfer Learning and Watershed Segmentation</a></h5>
  <h6>Build with:</h6>
</div>
<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/ISO_C%2B%2B_Logo.svg/1822px-ISO_C%2B%2B_Logo.svg.png" width="33" />
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/53/OpenCV_Logo_with_text.png/487px-OpenCV_Logo_with_text.png" width="33" />
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0a/Python.svg/1024px-Python.svg.png" width="33" />
  <img src="https://user-images.githubusercontent.com/40668801/42043955-fbb838a2-7af7-11e8-9795-7f890e871d13.png" width="33" />
</p>

## Introduction

This project uses watershed segmentation on images of optical microscope, to extract single cells from the image, label them, and retrain a ResNet-50 model to classify them into 3 classes, which are:
- Round
- Blob (Elongated Cell)
- Not resolvable
This project also aimed to test how well can pretrained model extract the features of cells, mainly their shape, to classify them correctly. </br>

The application works as follows:
* The images are preprocessed with Illumination Gradient and Noise removal (See App/Components/Transformations), the algorithms are explained in detail there
* The marker controlled watershed segmentation is applied, using Halo-Filter for cell center markers, and Histerisis thresholding for background marker extraction.
* The segmented image is the divided into N 224x224 patches, which were then used for the fine-tuning and training with Transfer Learning, of a ResNet-50 model

All the algorithms were writen in openCV c++. The model was trained on GoogleCollab.
<h6>Photo of steps of the algorithm: 1. Original cells, 2. After preprocessing algorithms, 3. Extracted Background, 4. Clache transformation, 5. Potential cell centers localization, 6. Halo-Filter rendering, and assigning one halo-center to one local-minima, with set radius minimum between them. Resulting in foreground watershed marker.</h6>

![result-min](https://github.com/user-attachments/assets/940ea240-f655-455a-88f3-6585830418ff)




## Results
What was discoverd is that the model reached 75% accuracy, while it is sufficient for most cases, with images where cells are together, the model shows it's limitations
The accuracy is justified with difficulties in segmentation. The cells blended with background and themselves, and their shape was inconsistent, therefore little mathematical help could be used to amplify the segmentation. Overall the application works well with images where cell walls/membrane is clearly visible, and cells are not blended. </br> </br>

What was also found surprising is that the model sometimes get's stuck in predicting all the time the same class. In some photos it returns always the same class. This issue is for now uneplainable by me. It happens rarely, and usually rerun of the model fixes the issue.

<h6>Example results. The class "Blob" is blue, "Round" is red, and black is "Not resolvable". As visible on attached Figure, the model performes significantly well when the cells are not in a touching distance, and are not blended with background.  For future work, might be added more classes (especially the star shape), but the segmentation process must be altered and improved. </h6>

<p align='center'>
  <img src="https://github.com/user-attachments/assets/a4dc5bd7-b993-481d-a431-1347768e48b1" width=800 />
</p>


## Project Architecture and description

> [!WARNING]
 Please be aware that this work uses ResNet-50 model architecture and Tensorflow library. The commercial use might be restricted, please read about it before commercial use!

> [!IMPORTANT]
> The model is not included in github repo. It must be downloaded from google drive. See section !Notes

The project is build as attached on the schema below. It is devided into seprate algorithms. Each algorithms has it's unique folder with .cpp file in `/App/Utils`.
These algorithms are run subsequently by `/App/Utils/Preprocess_All` algorithm, which controls the output and input of the algorithms.
The Preprocess_All algorithm can be executed from `App/EXECUTIONS/fire_segmentation.py`, which uses the .option file to set the queue of algorithms, decide the arguments on which they run, their order, and how the images are saved.
Each algorithm can be replaced (If it follows the same input, and output scheme).

<p align='center'>
  <img src="https://github.com/user-attachments/assets/9fa0fbdc-7b50-40e4-8ef7-6bafecd4c330" width=800 />
</p>

### Input and output scheme of single algorithm
Every algorithm follows that scheme of input, which is described in the 10 first files of each algorithm in it's .cpp file. Input must be provided as argumnets with the command. </br> </br>
Let's take example algorithm for noise reduction. It is in folder `/App/Utils/Norm_noise/main.cpp`. <strong> While being in terminal path in that folder </strong>, executing it requires providing
several arguments, example to run this algorithm is `./main.out 15 30 3 5 5 0 0 [./d1.jpg,./d2.jpg,./d3.jpg,./d4.jpg] ./temp_img`. The paths to images must be without any spaces in between them, they are divided by a comma ",". The output folder will be created by the algorithm if it does not exist. </br> </br>

Some algorithms does not take arguments as numbers, but as "def". They are not yet ready :). The def will be possibly in future replaced by a path to .option file.

### The `App/Utils/Preprocess_all/main.cpp` .option file

Running this file requires providing .option file. This file is structured to run other algorithms in an easy way. It takes as an argument 3 paths. An .option file path, a .txt file with files to alter, and output path (folder).
```
--default.option
--main.cpp
--paths.txt
--run.sh
--/output
```
The command to run this algorithms is then (after compiling the .cpp file, can be done on fedora with ./run.sh) `./main.out ./default.option ./paths.txt ./output`
The .option is described in `App/Utils/Preprocess_all/default_settings.option`. The "!ORG" makes it possible to work all the time on original files, making more subfolders

### The `App/EXECUTIONS/fire_segmentation.py` and other .py files
These files will be replaced by user GUI. they for now can be easily altered (comments inside them) to fast do preprocessing with `fire_preprocess.py` and segmentation with `fire_segmentation.py`. They use the same .option file to define which algorithms will be executed.

### The Image_Classifier App
Small app for classification of the images. Is not a part of application, but included here as helpful in fast labeling of images. Currently has a small error, and does a small leak of memory, which needs to be addressed.

## Note

* The project is not perfect, it may not work many times
* If you have trouble understanding the code or how to implemate something, fell free to ask
* The model itself is not included here, it can be downlaoded from here <a href="https://drive.google.com/drive/folders/16z6yjCrNShcm1pKsw9geb8CoyQRKD6Um?hl=pt-br">Google drive</a> file called avg.keras. Then it must be put to the folder: `App/Utils/Full_Segmentation/Full_model`. <strong>Otherwise, the app will not work</strong>

 ## License

If you reuse this, please put the author Tymoteusz Apriasz somewhere (you can add my instagram :) also).



