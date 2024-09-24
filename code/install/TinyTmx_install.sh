#!/bin/bash

# Install Zlib
cd ../external
mkdir zlib
cd zlib
# Download and install specific version of Zlib (1.2.13)
wget https://github.com/madler/zlib/archive/refs/tags/v1.2.13.zip
unzip v1.2.13.zip
# cd zlib-1.2.13
# ./configure
# make -j16
# sudo make install

# Install TinyXML2
cd ../
mkdir tinyxml2
cd tinyxml2
# Download and install specific version of TinyXML2 (9.0.0)
wget https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
tar -xzvf 9.0.0.tar.gz 
cd tinyxml2-9.0.0
mkdir build
# cd build
# cmake ..
# make -j 16
# sudo make install

# Install Zstd
cd ../..
mkdir zstd
cd zstd
# Download and install specific version of Zstd (1.5.2)
wget https://github.com/facebook/zstd/archive/refs/tags/v1.5.2.tar.gz 
tar -xzvf v1.5.2.tar.gz
# cd zstd-1.5.2
# make -j 16
# sudo make install
# cd build/cmake
# cmake .
# make  -j 16
# sudo make install



