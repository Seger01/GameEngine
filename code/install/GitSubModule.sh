#!/bin/bash

cd ../..

# Initialize and update submodules
git submodule update --init --recursive
if [ $? -ne 0 ]; then
    echo "Error: Failed to initialize and update submodules."
    exit 1
fi

echo "submodules initialized successfully."