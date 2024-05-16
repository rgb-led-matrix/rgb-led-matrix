add_subdirectory(Data_Protocol)
add_subdirectory(Control_Protocol)

add_library(RP2040_UART INTERFACE)

target_sources(RP2040_UART INTERFACE
    internal.cpp
    ../Protocol.cpp)

target_link_libraries(RP2040_UART INTERFACE
    RP2040_UART_Data_Protocol
    RP2040_UART_Control_Protocol
    Protocol
)
