#!/bin/sh
git submodule update --init
cd pico-sdk/
git submodule update --init
cd ..
DIR=$PWD
export PICO_SDK_PATH=$PWD/pico-sdk
cd RP2040_Module
mkdir build
cp ../pico-sdk/external/pico_sdk_import.cmake .
cd build
cmake ..
make -j $(($(nproc) * 2))
echo "Binary output:"
ls -la $DIR/RP2040_Module/build/mod.*
