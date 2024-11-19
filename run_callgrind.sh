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
    if grep -q "ENABLE_DEBUG:BOOL=OFF" CMakeCache.txt; then
        # If DEBUG is ON, disable tests and rebuild with debug options
        cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=OFF -DENABLE_TESTGAME=ON
    fi
fi

# Build with multiple cores and wait for it to complete
cmake --build . -- -j$(nproc) && \

rm -rf callgrind_out.txt callgrind_annotate.txt

cd -


# Run callgrind only after build is done
valgrind --tool=callgrind --simulate-cache=yes --callgrind-out-file=callgrind_run_out.txt ./code/build/bin/Engine_BRAVO_exec

# Open callgrind output
callgrind_annotate --auto=yes callgrind_run_out.txt >> callgrind_annotate.txt

mv callgrind_run_out.txt callgrind_annotate.txt code/build/
