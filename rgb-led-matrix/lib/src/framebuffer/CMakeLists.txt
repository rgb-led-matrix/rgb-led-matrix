add_library(framebuffer INTERFACE)

target_sources(framebuffer INTERFACE
    framebuffer.cpp
    RP2040/RP2040.cpp
    BCM/BCM.cpp)