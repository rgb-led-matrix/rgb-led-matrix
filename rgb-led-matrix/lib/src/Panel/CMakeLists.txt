add_library(Panel INTERFACE)

target_sources(Panel INTERFACE
    RGB/RGB.cpp
    MultiPanel_Internal.cpp
    Panel_Internal.cpp)
