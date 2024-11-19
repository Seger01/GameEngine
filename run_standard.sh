#!/bin/bash

BUILD_DIR="code/build"

# Check if build directory exists, create if not
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Check if CMakeCache.txt exists
if [ ! -f "CMakeCache.txt" ]; then
    cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=OFF -DENABLE_TESTGAME=ON
else
    # Check CMakeCache for ENABLE_DEBUG status
    if grep -q "ENABLE_DEBUG:BOOL=ON" CMakeCache.txt; then
        # If DEBUG is ON, disable tests and rebuild with debug options
        cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=OFF -DENABLE_TESTGAME=ON
    fi
fi

# Build with multiple cores
cmake --build . --target run -- -j$(nproc)
