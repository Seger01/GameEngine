#!/bin/bash

cd build

# Run the tests 
cmake .. -DENABLE_DEBUG=ON -DENABLE_TESTS=ON

cmake --build . --target run_tests -- -j$(nproc)

# Create an output directory for the coverage reports
rm -rf coverage_reports
mkdir -p coverage_reports

cd ..

# Generate the coverage report (ignoring external and build directories)
gcovr -e external -e build --html --html-details -o build/coverage_reports/coverage.html --print-summary --delete 
