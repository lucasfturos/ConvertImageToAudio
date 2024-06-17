# -------------------------------------------------------------------------

# CMake version check
cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

# -------------------------------------------------------------------------

# Define all project info
set(PROJECT_NAME
    ConvertImageToAudio
    LANGUAGES CXX
    VERSION 1.0.0  
)

# Create project "ConvertImageToAudio"
project(${PROJECT_NAME})

# Enable c++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags of compiling
add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g -Ofast
)

add_subdirectory(docs)
add_subdirectory(src)

# -------------------------------------------------------------------------