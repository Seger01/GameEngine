#!/bin/bash

# sudo apt-get install libsdl2-2.0-0 libsdl2-dev -y
cd ../external
if [ $? -ne 0 ]; then
    echo "Error: Failed to change directory to ../external."
    exit 1
fi

# Check if the SDL directory already exists
if [ -d "SDL" ]; then
    echo "SDL repository already exists. Skipping clone."
else
    git clone https://github.com/libsdl-org/SDL.git --branch release-2.30.7
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone SDL repository."
        exit 1
    fi
fi

echo "SDL2 download completed successfully."
