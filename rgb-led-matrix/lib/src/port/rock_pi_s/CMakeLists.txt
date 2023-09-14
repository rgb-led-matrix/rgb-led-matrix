add_library(rock_pi_s SHARED
    ../../RGBMatrix.cpp)

set(CMAKE_C_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

target_include_directories(rock_pi_s PUBLIC
    ../../../../include/
    ../../../include/
    ../../../../../external/FTDI/FTDI_D2XX/Linux/ARMv8_hard-float/)

target_link_directories(rock_pi_s PUBLIC
    ../../../../../external/FTDI/FTDI_D2XX/Linux/ARMv8_hard-float/build)

target_link_libraries(rock_pi_s
    Framebuffer
    CFG
    IO
    Panel
    ftd2xx
    pthread)

add_subdirectory(test)
