import tensorflow as tf
import cv2
import numpy as np

# Load the Keras model
model = tf.keras.models.load_model('./avg.keras')

# Function to preprocess the image (resize, normalize, etc.)
def preprocess_image(image_path, target_height=224, target_width=224):
    # Load the image using OpenCV
    img = cv2.imread(image_path)

    # Check if the image is loaded properly
    if img is None:
        print("Error loading image.")
        return None

    # Resize the image to match model's input size
    # img = cv2.resize(img, (target_width, target_height))

    # # Add a batch dimension (1, height, width, channels)
    img = np.expand_dims(img, axis=0)

    return img

# Function to make prediction
def make_prediction(model, image_path):
    # Preprocess the image
    img = preprocess_image(image_path)
    
    if img is None:
        return
    
    # Make the prediction
    predictions = model.predict(img)

    # Print the predictions (if classification, get the class with max probability)
    predicted_class = np.argmax(predictions, axis=1)  # For classification tasks
    print(f'Predicted class: {predicted_class}')

    # If it's a regression task, you can print the raw output
    # print(f'Predicted value: {predictions}')

# Example usage
image_path = './testts/aa.JPG'  # Path to your image
make_prediction(model, image_path)
