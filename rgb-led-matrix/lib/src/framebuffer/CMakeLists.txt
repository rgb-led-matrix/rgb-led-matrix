add_library(framebuffer INTERFACE)

target_sources(framebuffer INTERFACE
    framebuffer.cpp
    external/RP2040/RP2040_Multiplexed_Dual_UART.cpp
    HUB75/BCM/BCM.cpp)