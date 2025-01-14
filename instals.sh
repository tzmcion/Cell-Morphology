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

# Install dependencies and packages
if [[ "$DISTRO" == "ubuntu" ]]; then
    echo "Detected Ubuntu. Installing packages..."
    apt update && apt upgrade -y

    # Install OpenCV dependencies
    apt install -y build-essential cmake git libgtk-3-dev libboost-all-dev \
        libjpeg-dev libpng-dev libtiff-dev libopenexr-dev libwebp-dev libtbb-dev \
        libatlas-base-dev gfortran python3-dev

    # Install TensorFlow dependencies
    apt install -y python3-pip
    pip3 install --upgrade pip tensorflow

elif [[ "$DISTRO" == "fedora" ]]; then
    echo "Detected Fedora. Installing packages..."
    dnf update -y

    # Install OpenCV dependencies
    dnf install -y gcc-c++ cmake git gtk3-devel boost-devel \
        libjpeg-devel libpng-devel libtiff-devel openexr-devel \
        tbb-devel atlas-devel gcc-gfortran python3-devel

    # Install TensorFlow dependencies
    dnf install -y python3-pip
    pip3 install --upgrade pip tensorflow

else
    echo "Unsupported distribution: $DISTRO. Exiting."
    exit 1
fi

# Build and install OpenCV
OPENCV_VERSION="4.8.0"
mkdir -p /opt/opencv_build && cd /opt/opencv_build

git clone https://github.com/opencv/opencv.git -b $OPENCV_VERSION
cd opencv
mkdir build && cd build

cmake -D CMAKE_BUILD_TYPE=Release \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D BUILD_EXAMPLES=ON \
      ..
make -j$(nproc)
make install
ldconfig

# Confirm installations
python3 -c "import tensorflow as tf; print('TensorFlow version:', tf.__version__)"

if pkg-config --modversion opencv4; then
    echo "OpenCV installed successfully."
else
    echo "OpenCV installation failed."
fi

echo "Installation complete!"
