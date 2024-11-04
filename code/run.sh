#!/bin/bash

mkdir -p build

cd build

cmake ..


cmake --build . --target run -- -j$(nproc)


