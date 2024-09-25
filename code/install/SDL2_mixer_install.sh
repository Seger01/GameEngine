#!/bin/bash

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