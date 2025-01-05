from google.colab import drive
import tensorflow as tf
from tensorflow.keras import layers, models, optimizers, Sequential
from tensorflow.keras.preprocessing import image_dataset_from_directory
from tensorflow.keras.applications.resnet50 import ResNet50
from tensorflow.keras.callbacks import EarlyStopping
from tensorflow.keras.optimizers import Adam
import matplotlib.pyplot as plt
import numpy as np
###
###
###
IMAGE_SIZE = (224,224)
batch_size = 32
drive.mount('placeholder_path_google_drive')
dir_path = 'placehoolder_path_to_data_folders'
print(dir_path)
#dataset preparation
# Training dataset
dataset = image_dataset_from_directory(
    dir_path,
    labels='inferred',
    label_mode='categorical',
    color_mode='rgb',
    shuffle=True,
    validation_split=0.2,
    subset='training',
    image_size=IMAGE_SIZE,
    batch_size=batch_size,
    seed=4228
)

# Validation dataset
validation = image_dataset_from_directory(
    dir_path,
    labels='inferred',
    label_mode='categorical',
    color_mode='rgb',
    shuffle=True,
    validation_split=0.2,
    subset='validation',
    image_size=IMAGE_SIZE,
    batch_size=batch_size,
    seed=4228
)
###
###
###
#Improting the model
pretrained_model = ResNet50(
    input_shape=(224,224,3),
    weights="imagenet",
    include_top=False,
    pooling='avg')
model = Sequential()
###
###
###
for layer in pretrained_model.layers:
    layer.trainable = False
model.add(pretrained_model)
###
###
###
data_augmentations = Sequential([
    layers.RandomFlip("horizontal_and_vertical"),
    layers.RandomRotation(0.5),
    layers.RandomZoom(0.4),
    layers.RandomContrast(0.4),
    layers.RandomTranslation(0.3, 0.3),
    layers.GaussianNoise(0.4),
    layers.RandomBrightness(factor=0.3)
])
#using automated augmentation
dataset = dataset.map(lambda x, y: (data_augmentations(x), y))
dataset = dataset.cache().prefetch(buffer_size=tf.data.AUTOTUNE)
###
###
###
#Adding final layers
model.add(layers.Flatten())
model.add(layers.Dense(256, activation='relu'))
model.add(layers.Dropout(0.5))
model.add(layers.Dense(3, activation='softmax'))

#Early Stopping
early_stopping = EarlyStopping(
    monitor='val_loss',        # Monitor validation loss
    patience=150,                # Stop after 5 epochs with no improvement
    restore_best_weights=True  # Restore the best model weights
)

#Let's use learning rate 0.0001
model.compile(optimizer=Adam(learning_rate=0.0001), loss='categorical_crossentropy', metrics=['accuracy'])
history = model.fit(dataset, validation_data=validation, epochs=25, callbacks=[early_stopping])
#Unfreeze ayers after 25 epochs
for layer in pretrained_model.layers[-43:]:  # Unfreeze 43 last layers
    layer.trainable = True
#Lower learning rate
model.compile(optimizer=Adam(learning_rate=0.00001), loss='categorical_crossentropy', metrics=['accuracy'])
history_fine = model.fit(dataset, validation_data=validation, epochs=150, callbacks=[early_stopping])
###
###
###
#Evaluation
combined_history = {
    'accuracy': history.history['accuracy'] + history_fine.history['accuracy'],
    'val_accuracy': history.history['val_accuracy'] + history_fine.history['val_accuracy'],
    'loss': history.history['loss'] + history_fine.history['loss'],
    'val_loss': history.history['val_loss'] + history_fine.history['val_loss']
}

fig1 = plt.gcf()
plt.plot(combined_history['accuracy'])
plt.plot(combined_history['val_accuracy'])
plt.plot(combined_history['loss'])
plt.plot(combined_history['val_loss'])
plt.axis(ymin=0.3, ymax=7)
plt.grid()
plt.title('Model Accuracy')
plt.ylabel('Accuracy')
plt.xlabel('Epochs')
plt.legend(['train', 'validation','loss','validation_loss'])
plt.show()
