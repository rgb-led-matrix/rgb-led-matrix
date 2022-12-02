add_library(framebuffer INTERFACE)

target_sources(framebuffer INTERFACE
    framebuffer.cc
    RP2040/RP2040.cpp)