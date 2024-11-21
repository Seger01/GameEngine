#!/bin/bash

BUILD_DIR="code/build"

# Create build directory if it does not exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Check if CMakeCache.txt exists
if [ ! -f "CMakeCache.txt" ]; then
    cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=ON -DENABLE_TESTGAME=ON
else
    # Check CMakeCache for ENABLE_DEBUG status
    if grep -q "ENABLE_TESTS:BOOL=OFF" CMakeCache.txt; then
        # Switch to debug mode if currently in standard mode
        cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=ON -DENABLE_TESTGAME=ON
    fi
fi

# Build and run tests with multiple cores
cmake --build . --target run_tests -- -j$(nproc)

# Create or clear output directory for coverage reports
cd ..
COVERAGE_DIR="build/coverage_reports"
rm -rf "$COVERAGE_DIR"
mkdir -p "$COVERAGE_DIR"

# Generate the coverage report, excluding external and build directories
gcovr -e external -e build --html --html-details -o "$COVERAGE_DIR/coverage.html" --print-summary --delete
