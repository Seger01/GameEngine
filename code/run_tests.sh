#!/bin/bash

cd build

# Run the tests
bin/Engine_BRAVO_tests

# Create an output directory for the coverage reports
mkdir -p coverage_reports

cd ..

# Generate the coverage report (ignoring external and build directories)
gcovr -e external -e build --html --html-details -o build/coverage_reports/coverage.html --delete --txt build/coverage_reports/coverage.txt

# Print the coverage txt summary
cat build/coverage_reports/coverage.txt

# Delete the coverage note files (coverage data files are already deleted by --delete previous line)
find . -name "*.gcno" -type f -delete