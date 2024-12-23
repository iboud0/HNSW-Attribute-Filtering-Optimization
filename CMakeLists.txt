# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(HNSW-Attribute-Filtering-Optimization VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/roaring)

# Add definitions to fix AVX512 issue
add_definitions(-DCROARING_COMPILER_SUPPORTS_AVX512=0)

# Include Roaring amalgamated files directly if present
set(ROARING_SRC ${PROJECT_SOURCE_DIR}/src/roaring.c)

# Check if CPM.cmake exists and include it
if(EXISTS ${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
    include(cmake/CPM.cmake)

    # Fetch the CRoaring library
    CPMAddPackage(
      NAME roaring
      GITHUB_REPOSITORY RoaringBitmap/CRoaring
      GIT_TAG v2.1.0 # Use the latest stable version
    )
endif()

# Add executables and link Roaring
add_executable(example_filter src/example_filter.cpp ${ROARING_SRC})
target_link_libraries(example_filter PUBLIC roaring::roaring)

add_executable(bitset_filter src/bitset_filter.cpp ${ROARING_SRC})
target_link_libraries(bitset_filter PUBLIC roaring::roaring)

add_executable(linear_filter src/linear_filter.cpp ${ROARING_SRC})
target_link_libraries(linear_filter PUBLIC roaring::roaring)

add_executable(hnsw_optimization src/main.cpp ${ROARING_SRC})
target_link_libraries(hnsw_optimization PUBLIC roaring::roaring)

add_executable(test_roaring src/test_roaring.cpp ${ROARING_SRC})
target_link_libraries(test_roaring PUBLIC roaring::roaring)

add_executable(roaring_filter src/roaring_bitmaps_filter.cpp ${ROARING_SRC})
target_link_libraries(roaring_filter PUBLIC roaring::roaring)
