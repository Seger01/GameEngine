#!/bin/bash

mkdir -p build

cd build

cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=OFF

cmake --build . --target run -- -j$(nproc)