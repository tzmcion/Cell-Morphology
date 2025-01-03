import tensorflow as tf
import cv2
import numpy as np
import csv

# Load the Keras model
model = tf.keras.models.load_model('./avg.keras')

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
paths = './paths.csv'
with open(paths, newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in spamreader:
        image_path = row[0][:-1]
        pr_class = make_prediction(model,image_path)
        print(pr_class)
