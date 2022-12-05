add_library(framebuffer INTERFACE)

target_sources(framebuffer INTERFACE
    framebuffer.cpp
    external/RP2040/RP2040.cpp
    HUB75/BCM/BCM.cpp)