#!/bin/bash

sudo apt install -y cmake git gcc-arm-none-eabi gcc g++ build-essential groovy python3 doxygen graphviz
sudo mkdir /opt/pi
sudo chown $(whoami) /opt/pi
cd /opt/pi
git clone https://github.com/raspberrypi/tools