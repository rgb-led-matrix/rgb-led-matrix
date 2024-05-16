add_library(RP2040_UART_Data_Protocol INTERFACE)

target_sources(RP2040_UART_Data_Protocol INTERFACE
    Data.cpp
    Query.cpp
    Status.cpp
    RP2040_UART_Data.cpp)
