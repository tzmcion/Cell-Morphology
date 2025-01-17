#!/bin/bash

# Check for root privileges
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

# Detect the Linux distribution
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO=$ID
else
    echo "Unsupported distribution. Exiting."
    exit 1
fi

# Install packages
if [[ "$DISTRO" == "ubuntu" ]]; then
    echo "Detected Ubuntu. Installing packages..."
    apt update && apt upgrade -y

    # Install CMake, g++, TensorFlow, and OpenCV
    apt install -y cmake g++ python3-pip libopencv-dev libopencv-contrib-dev

    # Install TensorFlow
    pip3 install --upgrade pip tensorflow

elif [[ "$DISTRO" == "fedora" ]]; then
    echo "Detected Fedora. Installing packages..."
    dnf update -y

    # Install CMake, g++, TensorFlow, and OpenCV
    dnf install -y cmake gcc-c++ python3-pip opencv opencv-devel opencv-contrib

    # Install TensorFlow
    pip3 install --upgrade pip tensorflow opencv-python-headless

else
    echo "Unsupported distribution: $DISTRO. Exiting."
    exit 1
fi

# Confirm installations
python3 -c "import tensorflow as tf; print('TensorFlow version:', tf.__version__)"

if pkg-config --modversion opencv4; then
    echo "OpenCV installed successfully."
else
    echo "OpenCV installation failed."
fi

echo "Installation complete!"
