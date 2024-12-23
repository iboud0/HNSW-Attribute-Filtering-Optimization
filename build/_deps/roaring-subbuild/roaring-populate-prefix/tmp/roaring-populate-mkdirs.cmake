# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-src"
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-build"
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix"
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix/tmp"
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix/src/roaring-populate-stamp"
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix/src"
  "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix/src/roaring-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix/src/roaring-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/roaring-subbuild/roaring-populate-prefix/src/roaring-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
