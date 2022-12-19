#!/bin/bash


sudo apt install -y cmake git gcc-arm-none-eabi gcc g++ build-essential groovy python3 doxygen graphviz wget

sudo mkdir /opt/rock-pi-s
sudo chown $(whoami) /opt/rock-pi-s

wget https://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/aarch64-linux-gnu/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu.tar.xz
tar xvf gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu.tar.xz  -C /opt/rock-pi-s

rm gcc-linaro-*