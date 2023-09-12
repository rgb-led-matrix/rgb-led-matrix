add_subdirectory(Node)
add_subdirectory(Root)

add_library(FTDI INTERFACE)

target_link_libraries(FTDI INTERFACE
    FTDI_UART
    FT4232)

add_library(IO INTERFACE)

target_link_libraries(IO INTERFACE
    FTDI_UART)
