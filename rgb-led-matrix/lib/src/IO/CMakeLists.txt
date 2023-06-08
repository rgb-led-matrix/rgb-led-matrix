# Build for ARMv6 with hard float
add_library(io-armv6-hf INTERFACE)

target_include_directories(io-armv6-hf INTERFACE
    ../../../../external/FTDI/FTDI_D2XX/Linux/ARMv6_hard-float/)

target_sources(io-armv6-hf INTERFACE
    Node/FTDI_UART.cpp
    Root/FT4232.cpp)

target_link_directories(io-armv6-hf INTERFACE
    ../../../../external/FTDI/FTDI_D2XX/Linux/ARMv6_hard-float/build
)

target_link_libraries(io-armv6-hf INTERFACE
    ftd2xx)

# Build for ARMv7 with hard float
add_library(io-armv7-hf INTERFACE)

target_include_directories(io-armv7-hf INTERFACE
    ../../../../external/FTDI/FTDI_D2XX/Linux/ARMv7_hard-float/)

target_sources(io-armv7-hf INTERFACE
    Node/FTDI_UART.cpp
    Root/FT4232.cpp)

target_link_directories(io-armv7-hf INTERFACE
    ../../../../external/FTDI/FTDI_D2XX/Linux/ARMv7_hard-float/build
)

target_link_libraries(io-armv7-hf INTERFACE
    ftd2xx)

# Build for ARMv8 with hard float
add_library(io-armv8-hf INTERFACE)

target_include_directories(io-armv8-hf INTERFACE
    ../../../../external/FTDI/FTDI_D2XX/Linux/ARMv8_hard-float/)

target_sources(io-armv8-hf INTERFACE
    Node/FTDI_UART.cpp
    Root/FT4232.cpp)

target_link_directories(io-armv8-hf INTERFACE
    ../../../../external/FTDI/FTDI_D2XX/Linux/ARMv8_hard-float/build
)

target_link_libraries(io-armv8-hf INTERFACE
    ftd2xx)
