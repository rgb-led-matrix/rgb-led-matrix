add_subdirectory(FTDI_UART)

add_library(Node INTERFACE)

target_sources(Node INTERFACE
    Node.cpp)