#!/bin/bash

cd external/RP2040/LED_Matrix_RP2040
git submodule update --init
groovy build.groovy -c cfg.xml
./LED_Matrix/build/build.sh
cd ../../../

cd rgb-led-matrix
mkdir build
cd build
cmake ..
make -j4