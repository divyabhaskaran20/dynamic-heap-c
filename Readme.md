# Custom Memory Allocator

##  Overview
This project implements a custom dynamic memory allocator in C, mimicking the functionality of `malloc()` and `free()` using a static heap. The implementation manages memory allocation, deallocation, and chunk merging to optimize memory usage.

```
+---------------------+--------------------------+
|   Metadata (16B)    |   Memory Chunk (N bytes) |
+---------------------+--------------------------+
| Size (4B)           |  User-allocated data     |
| Free (1B)           |                          |
| Padding (3B)        |                          |
| Next (8B)           |                          |
+---------------------+--------------------------+
The Metadata (16B) section includes information about the chunk's size, its free status, padding for alignment, and a pointer to the next chunk.
The Memory Chunk (N bytes) represents the user-allocated memory and the associated metadata.
```

## Features

✅ **Custom `malloc()` Implementation** - Dynamically allocates memory from a statically managed heap.  
✅ **Custom `free()` Implementation** - Releases allocated memory and merges adjacent free chunks.  
✅ **Memory Splitting & Merging** - Optimizes memory usage by splitting large chunks and merging adjacent free chunks.  
✅ **Heap Visualization** - Displays the current memory layout after each operation.  
✅ **Alignment Handling** - Ensures memory is properly aligned based on system architecture.  


##  Project Structure

The project is organized into the following directory structure:

```
├── build                   - Contains the build output, including the Makefile and the final executable (`custom_malloc_test_exe).
├── build_script.sh         - A shell script that automates the build process using CMake
├── CMakeLists.txt
├── include                 -  Contains header files for the project
│   ├── custom_malloc.h
│   └── custom_sbrk.h
├── main.c                   -  The main program that tests the custom memory allocator 
├── Readme.md                - Project documentation.
└── src                      -  Contains the source code for the custom memory allocator
    ├── custom_malloc.c
    └── custom_sbrk.c
```

## ️ How to Build and Run with CMake

### 1. **Clone the repository**:

```bash
git clone https://github.com/your-username/custom_malloc.git
cd custom_malloc
```
### 2. **Build the Project using cmake**:
```
Install Cmake 
sudo apt install cmake

This will automatically build the project using CMake and run the resulting executable.
./build_script.sh
```
### 3. **Run the Program**:
```
cd build
./custom_malloc_test_exe
```
### License
This project is open-source and available under the MIT License.


