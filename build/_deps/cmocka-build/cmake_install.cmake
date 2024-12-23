# Install script for directory: C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/HNSW-Attribute-Filtering-Optimization")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/mingw64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "pkgconfig" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/cmocka.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cmocka" TYPE FILE FILES
    "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/cmocka-config.cmake"
    "C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/cmocka-config-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/include/cmake_install.cmake")
  include("C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/src/cmake_install.cmake")
  include("C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/doc/cmake_install.cmake")
  include("C:/Code/HNSW-Attribute-Filtering-Optimization/build/_deps/cmocka-build/example/cmake_install.cmake")

endif()

