add_subdirectory(FTDI_UART)

add_library(Node INTERFACE)

target_link_libraries(Node INTERFACE
    FTDI_UART)
