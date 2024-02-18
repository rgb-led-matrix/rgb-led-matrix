add_library(Mapper INTERFACE)

target_sources(Mapper INTERFACE
    Pixel/Pixel.cpp
    Pixel/Outdoor/Outdoor.cpp)
