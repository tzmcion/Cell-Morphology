import tensorflow as tf
import cv2
import numpy as np
import time
import uuid
# Load the Keras model
model = tf.keras.models.load_model('../Full_Segmentation/Full_model/avg.keras')

# Function to preprocess the image (resize, normalize, etc.)
def preprocess_image(image_path):
    img = cv2.imread(image_path)
    if img is None:
        print("Error loading image.")
        return None
    img = np.expand_dims(img, axis=0)

    return img

# Function to make prediction
def make_prediction(model, image_path):
    img = preprocess_image(image_path)
    if img is None:
        return
    predictions = model.predict(img)
    predicted_class = np.argmax(predictions, axis=1)
    return predicted_class

# 1 - Not resolvable
# 2 - Round
# 0 - Blob
# paths = './paths.csv'
# with open(paths, newline='') as csvfile:
#     spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
#     for row in spamreader:
#         image_path = row[0][:-1]
#         pr_class = make_prediction(model,image_path)
#         print(pr_class)
accepted_types = ['.jpg', '.JPG', '.png', '.PNG']
path = "../Full_Segmentation/Full_model/to_predict.txt"
result_path = "../Full_Segmentation/Full_model/prediction_result.txt"
last_content = ""
pr_result = open(result_path, 'w')
pr_result.write("script ready") 
pr_result.close()
while last_content != "!STOP!":
    file = open(path, 'r')
    content = file.read().strip()
    if(content != last_content):
        last_content = content
        if last_content == "!STOP!":
            break
        extension = last_content[len(last_content)-4:len(last_content)]
        if(extension not in accepted_types):
            continue
        result = make_prediction(model,last_content)
        pr_result = open(result_path, 'w')
        ud = str(uuid.uuid4())
        if(str(result[0]) == "0"):
            pr_result.write(str("Blob " + ud))
        if(str(result[0]) == "1"):
            pr_result.write(str("Not Resolvable " + ud))
        if(str(result[0]) == "2"):
            pr_result.write(str("Round "+ ud))        
        pr_result.close()
    last_content = content
    file.close()
    time.sleep(0.02)
pr_result = open(result_path, 'w')
pr_result.write("file_closed") 
pr_result.close()
        
