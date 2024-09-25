#!/bin/bash

cd ../external
if [ $? -ne 0 ]; then
    echo "Error: Failed to change directory to ../external."
    exit 1
fi

# Check if the tmxlite directory already exists
if [ -d "tmxlite" ]; then
    echo "tmxlite repository already exists. Skipping clone."
else
    git clone https://github.com/fallahn/tmxlite.git --branch v1.4.4
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone tmxlite repository."
        exit 1
    fi
fi

echo "tmxlite download completed successfully."