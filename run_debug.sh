#!/bin/bash

BUILD_DIR="code/build"

# Check if build directory exists, create if not
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Check CMakeCache for ENABLE_DEBUG status
if grep -q "ENABLE_DEBUG:BOOL=OFF" CMakeCache.txt; then
    # Switch to debug mode if currently in standard mode
    cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=OFF
fi

# Build and run with multiple cores
cmake --build . --target run -- -j$(nproc)
