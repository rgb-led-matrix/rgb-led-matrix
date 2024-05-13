add_library(RP2040_UART INTERFACE)

target_sources(RP2040_UART INTERFACE
    RP2040_UART.cpp
    Status.cpp
    Data.cpp
    internal.cpp
    ../Protocol.cpp)
