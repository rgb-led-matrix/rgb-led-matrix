add_library(Panel INTERFACE)

target_sources(Panel INTERFACE
    Mapper/Pixel_Mapper/Pixel_Mapper.cpp
    MultiPanel.cpp)

# Lightweight version which does not use std::thread, std::vector, std::mutex, etc.
#   Useful for PIC32, Cortex-M7, etc.
add_library(Panel-light INTERFACE)

target_sources(Panel-light INTERFACE
    Panel_Pixel_Mapper.cpp)