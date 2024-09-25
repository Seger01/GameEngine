#!/bin/bash

cd ../external
if [ $? -ne 0 ]; then
    echo "Error: Failed to change directory to ../external."
    exit 1
fi

# Check if the SDL_mixer directory already exists
if [ -d "SDL_mixer" ]; then
    echo "SDL_mixer repository already exists. Skipping clone."
else
    git clone https://github.com/libsdl-org/SDL_mixer.git --branch release-2.8.0
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone SDL_mixer repository."
        exit 1
    fi
fi

# Install OpusFile library
sudo apt-get install -y libopusfile-dev

# Check if the installation was successful
if [ $? -ne 0 ]; then
    echo "Failed to install OpusFile library"
    exit 1
fi

# Install libxmp library
sudo apt-get install -y libxmp-dev

# Check if the installation was successful
if [ $? -ne 0 ]; then
    echo "Failed to install libxmp library"
    exit 1
fi

# Install wavpack library
sudo apt-get install -y libwavpack-dev

# Check if the installation was successful
if [ $? -ne 0 ]; then
    echo "Failed to install libwavpack-dev library"
    exit 1
fi

echo "SDL2_mixer download completed successfully."