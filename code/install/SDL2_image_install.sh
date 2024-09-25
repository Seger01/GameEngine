#!/bin/bash

# Navigate to the external directory
cd ../external
if [ $? -ne 0 ]; then
    echo "Error: Failed to change directory to ../external."
    exit 1
fi

# Check if the SDL_image directory already exists
if [ -d "SDL_image" ]; then
    echo "SDL_image repository already exists. Skipping clone."
else
    git clone https://github.com/libsdl-org/SDL_image.git --branch release-2.8.2
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone SDL_image repository."
        exit 1
    fi
fi

echo "SDL_image download completed successfully."