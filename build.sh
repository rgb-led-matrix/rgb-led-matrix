#!/bin/bash

cd rgb-led-matrix
mkdir build
cd build
cmake ..
make -j8
