#!/bin/bash

mkdir -p build

cd build

# Remove existing coverage files
rm -rf CMakeFiles/Engine_BRAVO_lib.dir/src/*.gcda
rm -rf CMakeFiles/Engine_BRAVO_lib.dir/src/*.gcno

# Generate the build files
cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=ON 

# Build the project
cmake --build . -- -j$(nproc)
