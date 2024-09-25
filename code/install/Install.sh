#!/bin/bash

# Update package list and upgrade system packages
sudo apt-get update
sudo apt-get upgrade -y

# Fix broken dependencies
sudo apt-get install -f -y

# Clean up package lists
sudo apt-get autoremove -y
sudo apt-get clean

# Function to run a script and check its return code
run_script() {
    local script=$1
    ./$script
    if [ $? -ne 0 ]; then
        echo "Error: $script failed."
        exit 1
    fi
}

# Call all install files with error checking
run_script SDL2_install.sh
run_script SDL2_Mixer.sh
run_script SLikeNet_install.sh
run_script TmxLite_install.sh
run_script Box2D_install.sh

echo "All dependencies installed/downloaded successfully"
exit 0