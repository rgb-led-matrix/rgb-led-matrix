add_subdirectory(Data_Operation)
add_subdirectory(Query_Request)

add_library(RP2040_UART_Data_Protocol INTERFACE)

target_sources(RP2040_UART_Data_Protocol INTERFACE
    Data.cpp
    Query.cpp
    Status.cpp
    Status_msg.cpp
    RP2040_UART_Data.cpp)

target_link_libraries(RP2040_UART_Data_Protocol INTERFACE
    RP2040_UART_Data_Operation
    RP2040_UART_Query_Request
)
