add_library(rock_pi_s SHARED
    ../../led-matrix.cpp
    ../../framecanvas.cpp)

set(CMAKE_C_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/rock-pi-s/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-g++)

target_link_libraries(rock_pi_s
    pixel-mapper
    multiplex-mapper
    framebuffer
    pin-pulser
    worker)
