#!/bin/bash

cd ../external
if [ $? -ne 0 ]; then
    echo "Error: Failed to change directory to ../external."
    exit 1
fi

# Check if the SLikeNet directory already exists
if [ -d "SLikeNet" ]; then
    echo "SLikeNet repository already exists. Skipping clone."
else
    git clone https://github.com/SLikeSoft/SLikeNet.git --branch v.0.1.3
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone SLikeNet repository."
        exit 1
    fi
fi

echo "SLikeNet download completed successfully."