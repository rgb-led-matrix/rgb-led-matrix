add_library(framebuffer INTERFACE)

target_sources(framebuffer INTERFACE
    Framebuffer.cpp
    PixelDesignatorMap.cpp
    external/RP2040//RP2040.cpp)