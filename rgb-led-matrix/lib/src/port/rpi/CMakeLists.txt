add_library(rpi SHARED
    gpio/gpio.cc
    pin-mapper/hardware-mapping.c
    pin-pulser/pin-pulser.cpp
    thread/thread.cc
    ../../led-matrix.cc
    ../../framecanvas.cc)

set(CMAKE_C_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

target_link_libraries(rpi
    pixel-mapper
    multiplex-mapper
    BCM)
