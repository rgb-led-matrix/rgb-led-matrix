add_subdirectory(Node)
add_subdirectory(Root)
add_subdirectory(Protocol)

add_library(IO INTERFACE)

target_link_libraries(IO INTERFACE
    )
