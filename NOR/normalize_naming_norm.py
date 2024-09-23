#Script to normalize naming in HK
#Works only once, when files are deleted or moved it will throw an exception or error in worst cases.

import os
import shutil

paths = [f.path for f in os.scandir("./") if f.is_dir()]

for path in paths:
    if path == './TIMELINEIMAGES':
        continue
    path_final = path + "/POINT 00001"
    # deep_paths = [f.path for f in os.scandir(path_final)]
    # count = 0
    # for p in deep_paths:
    #     photo = str(p)
    #     photo_new_name = str(path + "/{:05d}".format(count) + ".JPG")
    #     count += 1
    #     os.rename(photo,photo_new_name)
    shutil.rmtree(path_final)
