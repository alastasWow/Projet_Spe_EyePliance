# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)

PROJECT( eyelrecon )
FIND_PACKAGE( OpenCV REQUIRED )
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3 -w")

add_subdirectory(src)

#si vous voulez compiler un fichier avec le linkage avec la librairie opencv
#g++ `pkg-config --cflags opencv` sample.cpp `pkg-config --libs opencv`
