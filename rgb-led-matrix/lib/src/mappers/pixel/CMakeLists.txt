add_subdirectory(HUB75)

add_library(pixel-mapper INTERFACE)

target_link_libraries(pixel-mapper INTERFACE
    pixel-mapper-HUB75)