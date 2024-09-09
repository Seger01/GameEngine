#!/bin/bash
# sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-2.0-0 libsdl2-dev -y

cd ..

cd SDL
mkdir build
cd build
../configure
make -j 16
sudo make install
