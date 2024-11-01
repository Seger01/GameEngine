#!/bin/bash

mkdir build 
cd build 
cmake ..
cd ..

make -C build/ -j 32 & ./build/bin/engine_BRAVO


