#!/bin/bash

#create slikenet dir and source dir
cd ..
mkdir slikenet
mkdir slikenet/source

#download and unzip slikenet
cd slikenet/source
wget https://www.slikesoft.com/download/slikenet_0.1.3_source.tar.gz
tar -xvf slikenet_0.1.3_source.tar.gz

#create cmake dir
cd ..
mkdir cmake
cd cmake

#call cmake
cmake ../source/slikenet

#call make
make -j 16
