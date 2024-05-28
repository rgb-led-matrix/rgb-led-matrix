add_library(RP2040_UART_Query_Request INTERFACE)

target_sources(RP2040_UART_Query_Request INTERFACE
    Query_Temperature.cpp)