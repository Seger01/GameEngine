#!/bin/bash

# Navigate to the external directory
cd ../external
if [ $? -ne 0 ]; then
    echo "Error: Failed to change directory to ../external."
    exit 1
fi

# Check if the box2d directory already exists
if [ -d "box2d" ]; then
    echo "Box2D repository already exists. Skipping clone."
else
    git clone https://github.com/erincatto/box2d.git --branch v3.0.0
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone Box2D repository."
        exit 1
    fi
fi

echo "Box2D download completed successfully."