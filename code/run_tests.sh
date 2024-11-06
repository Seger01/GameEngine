#!/bin/bash

cd build

# Run the tests
bin/Engine_BRAVO_tests

# Create an output directory for the coverage reports
rm -rf coverage_reports
mkdir -p coverage_reports

cd ..

# Generate the coverage report (ignoring external and build directories)
gcovr -e external -e build --html --html-details -o build/coverage_reports/coverage.html --print-summary --delete