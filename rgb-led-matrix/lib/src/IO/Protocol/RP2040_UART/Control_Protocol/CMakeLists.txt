add_library(RP2040_UART_Control_Protocol INTERFACE)

target_sources(RP2040_UART_Control_Protocol INTERFACE
    Control.cpp
    RP2040_UART.cpp)
