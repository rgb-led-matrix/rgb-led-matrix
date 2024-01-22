add_library(rock_pi_s SHARED
    ../../RGBMatrix.cpp)

set(CMAKE_C_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

target_include_directories(rock_pi_s PUBLIC
    ../../../../include/
    ../../../include/)

target_link_libraries(rock_pi_s
    Framebuffer
    CFG
    IO
    Panel
    pthread)

add_subdirectory(test)
