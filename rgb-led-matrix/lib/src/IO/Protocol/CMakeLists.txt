add_subdirectory(RP2040_UART)

add_library(Protocol INTERFACE)

target_sources(Protocol INTERFACE
    Data_Protocol.cpp
    Control_Protocol.cpp)
