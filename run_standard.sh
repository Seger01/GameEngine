#!/bin/bash

mkdir code/build

cd code/build

cmake .. -DENABLE_DEBUG=OFF -DENABLE_TESTS=OFF

cmake --build . --target run -- -j$(nproc)