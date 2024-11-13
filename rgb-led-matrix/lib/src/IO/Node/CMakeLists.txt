add_subdirectory(FTDI_UART)
add_subdirectory(Test_Node)

add_library(Node INTERFACE)

target_sources(Node INTERFACE
    Node.cpp)