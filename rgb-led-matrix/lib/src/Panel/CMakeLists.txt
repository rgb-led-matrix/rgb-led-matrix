add_library(Panel INTERFACE)

target_sources(Panel INTERFACE
    Mapper/Pixel/Pixel.cpp
    MultiPanel.cpp)
