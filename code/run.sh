#!/bin/bash

mkdir build 
cd build 
cmake ..
cd ..

make -C build/ -j 16

# ./engine_BRAVO
./build/bin/engine_BRAVO

