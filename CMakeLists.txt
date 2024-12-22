# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(HNSW-Attribute-Filtering-Optimization VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
add_executable(hnsw_optimization src/main.cpp)
add_executable(example_filter src/example_filter.cpp)

