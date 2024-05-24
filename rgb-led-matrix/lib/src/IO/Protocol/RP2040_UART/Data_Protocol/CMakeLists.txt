add_library(RP2040_UART_Data_Protocol INTERFACE)

target_sources(RP2040_UART_Data_Protocol INTERFACE
    Data.cpp
    Data_Worker.cpp
    Query.cpp
    Status.cpp
    Status_msg.cpp
    RP2040_UART_Data.cpp)
