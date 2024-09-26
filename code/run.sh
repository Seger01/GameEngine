#!/bin/bash

mkdir build 
cd build 
cmake ..
cd ..

make -C build/ -j 32

# ./engine_BRAVO
./build/bin/engine_BRAVO

