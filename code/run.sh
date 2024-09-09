#!/bin/bash

mkdir build 
cd build 
cmake ..
cmake --build . -- -j 16

cd ..

# ./engine_BRAVO
./build/engine_BRAVO

