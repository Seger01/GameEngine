#!/bin/bash

mkdir -p build
cd build || { echo "Failed to enter build directory"; exit 1; }
cmake .. || { echo "CMake configuration failed"; exit 1; }
make -j 32 || { echo "Build failed"; exit 1; }
cd ..

./build/bin/engine_BRAVO || { echo "Failed to run the program"; exit 1; }
