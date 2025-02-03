#!/bin/bash

# Check if the build directory exists
if [ -d "build" ]; then
  # If it exists, remove it to clean the build
  echo "Cleaning previous build..."
  rm -rf build
fi

# Create a new build directory
mkdir build

# Run CMake to configure the project
cmake . -B build 

# Compile the project using make
make -C build

# Print a message indicating successful build
echo "Build completed successfully!"

# Optionally, run the executable after building
#./build/custom_malloc_test_exe 
