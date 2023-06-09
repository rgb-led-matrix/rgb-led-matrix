add_subdirectory(external)

add_library(framebuffer INTERFACE)

target_sources(framebuffer INTERFACE
    Framebuffer.cpp
    PixelDesignatorMap.cpp)

target_link_libraries(framebuffer INTERFACE
    RP2040_UART)