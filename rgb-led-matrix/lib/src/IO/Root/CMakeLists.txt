add_subdirectory(FT4232)

add_library(Root INTERFACE)

target_link_libraries(Root INTERFACE
    FT4232)
