add_library(FTDI_UART INTERFACE)

target_sources(FTDI_UART INTERFACE
    FTDI_UART.cpp)

target_link_libraries(FTDI_UART INTERFACE
    Node)
