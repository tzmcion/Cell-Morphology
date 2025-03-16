#!/bin/bash

# Compiler
CXX=g++

# Compiler flags (add the components folder to the include path)
CXXFLAGS="-std=c++17 -Wall `pkg-config --cflags opencv4` -I./components"

# Linker flags (OpenCV libraries)
LDFLAGS="`pkg-config --libs opencv4`"

# Target executable name
TARGET="main.out"

# List of source files (including .cpp files from the /components directory)
SRCS="main.cpp $(find ../../Components -name "*.cpp")"

# List of object files (convert .cpp to .o)
OBJS=$(echo "$SRCS" | sed 's/\.cpp/\.o/g')

# Compile .cpp files into .o object files
echo "Compiling source files..."
for src in $SRCS; do
    obj=$(echo $src | sed 's/\.cpp/\.o/g')
    echo "Compiling $src..."
    $CXX $CXXFLAGS -c $src -o $obj
done

# Link object files into the final executable
echo "Linking object files..."
$CXX -o $TARGET $OBJS $LDFLAGS

# Clean up object files after compilation (optional)
echo "Cleaning up object files..."
rm -f $OBJS

echo "Build complete. Executable: $TARGET"
