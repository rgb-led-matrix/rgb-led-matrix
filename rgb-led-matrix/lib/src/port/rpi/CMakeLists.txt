add_library(rpi SHARED
    ../../RGBMatrix.cpp)

set(CMAKE_C_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

target_include_directories(rpi PUBLIC
    ../../../../include/
    ../../../include/
    ../../../../../external/FTDI/FTDI_D2XX/Linux/ARMv6_hard-float/)

target_link_directories(rpi PUBLIC
    ../../../../../external/FTDI/FTDI_D2XX/Linux/ARMv6_hard-float/build)

target_compile_options(rpi PRIVATE
    -Wall
    -Werror
   -O3
)

target_link_libraries(rpi
    Exception
    ThreadPool
    CFG
    IO
    Panel
    Frame
    Logger
    ftd2xx
    pthread)

add_subdirectory(test)
