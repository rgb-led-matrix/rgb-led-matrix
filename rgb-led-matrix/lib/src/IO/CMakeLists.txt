add_subdirectory(Node)
add_subdirectory(Scheduler)
add_subdirectory(Protocol)

add_library(FTDI INTERFACE)

target_link_libraries(FTDI INTERFACE
    FTDI_UART
    RP2040_UART)

add_library(IO INTERFACE)

target_link_libraries(IO INTERFACE
    FTDI
    Scheduler)
