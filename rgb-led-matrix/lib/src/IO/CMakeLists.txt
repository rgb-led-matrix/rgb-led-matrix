add_subdirectory(Node)
add_subdirectory(Root)

add_library(IO INTERFACE)

target_link_libraries(IO INTERFACE
    Node
    Root)
