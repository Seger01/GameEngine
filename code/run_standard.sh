#!/bin/bash

mkdir -p build

cd build

cmake .. -DENABLE_DEBUG=OFF -DENABLE_TESTS=OFF

cmake --build . --target run -- -j$(nproc)