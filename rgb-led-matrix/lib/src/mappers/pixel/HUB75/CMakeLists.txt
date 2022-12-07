add_library(pixel-mapper-HUB75 INTERFACE)

target_sources(pixel-mapper-HUB75 INTERFACE
    pixel-mapper.cc
    PixelMapper_HUB75_LUT.cpp)