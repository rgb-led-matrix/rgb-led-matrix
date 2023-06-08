add_library(rpi SHARED
    ../../RGBMatrix.cpp)

set(CMAKE_C_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

target_include_directories(rpi PRIVATE
    ../../../../include/
    ../../../include/)

target_link_libraries(rpi
    multiplex-mapper
    framebuffer
    CFG
    Panel
    io-armv6-hf)
