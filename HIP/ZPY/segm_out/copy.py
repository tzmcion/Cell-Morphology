import os
import random
import shutil
import uuid

# Define source and destination paths
source_base = "."  # Assuming script.py is in the same directory as A01, A02, etc.
dest_base = "/home/tymoteuszapriasz/KM/classification/to_classify"
os.makedirs(dest_base, exist_ok=True)  # Ensure destination folder exists
folder_path = [f.path for f in os.scandir(source_base) ]
idx = 0
# Loop through folders A01 to A12
for i in folder_path:
    folder_name = i
    full_segmentation_path = os.path.join(folder_name, "Full_Segmentation")
    if os.path.exists(full_segmentation_path):
        # Get all images in the Full_Segmentation folder
        all_folders = [f.path for f in os.scandir(full_segmentation_path) ]
        for folder in all_folders:
            if(os.path.isdir(folder)):
                all_paths = [f.path for f in os.scandir(folder)]
                # Select 400 images randomly
                selected_images = random.sample(all_paths, min(20, len(all_paths)))
                print(selected_images)
                for image in selected_images:
                    src = image
                    dest = os.path.join(dest_base, "img_" + str(uuid.uuid4()) + ".JPG")  # Rename to avoid overwrites
                    shutil.copy(src, dest)
                    idx+=1

print("Images have been successfully copied.")
