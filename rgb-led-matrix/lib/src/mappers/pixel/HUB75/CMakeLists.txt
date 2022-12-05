add_library(pixel-mapper INTERFACE)

target_sources(pixel-mapper INTERFACE
    pixel-mapper.cc
    PixelMapper_HUB75_LUT.cpp)