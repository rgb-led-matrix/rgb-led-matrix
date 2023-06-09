add_library(rock_pi_s SHARED
    ../../RGBMatrix.cpp)

set(CMAKE_C_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

target_include_directories(rock_pi_s PRIVATE
    ../../../../include/
    ../../../include/
    ../../../../../external/FTDI/FTDI_D2XX/Linux/ARMv8_hard-float/)

target_link_directories(rock_pi_s PRIVATE
    ../../../../../external/FTDI/FTDI_D2XX/Linux/ARMv8_hard-float/build)

target_link_libraries(rock_pi_s
    multiplex-mapper
    framebuffer
    CFG
    Panel
    IO
    ftd2xx)
