#!/bin/bash

mkdir -p build

cd build

cmake .. -DENABLE_DEBUG=ON

cmake --build . --target run -- -j$(nproc)