add_library(rpi SHARED
    ../../led-matrix.cpp
    ../../framecanvas.cpp)

set(CMAKE_C_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

target_link_libraries(rpi
    pixel-mapper
    multiplex-mapper
    framebuffer
    pin-pulser
    worker)
