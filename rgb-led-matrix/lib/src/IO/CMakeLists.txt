add_library(io INTERFACE)

target_sources(io INTERFACE
    Node/FTDI_UART.cpp
    Root/FT4232.cpp)