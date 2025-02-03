cmake_minimum_required(VERSION 3.10)

# Set project name and specify C as the language
project(dynamic_memory_allocation C)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the executable
add_executable(custom_malloc_test_exe
    main.c 
    src/custom_sbrk.c 
    src/custom_malloc.c 
)

# Include directories
target_include_directories(custom_malloc_test_exe PRIVATE include)

# Enable warnings
target_compile_options(custom_malloc_test_exe PRIVATE -Wall -Wextra -Wpedantic)

