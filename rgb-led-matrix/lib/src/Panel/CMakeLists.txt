add_library(Panel INTERFACE)

target_sources(Panel INTERFACE
    MultiPanel_Internal.cpp
    Single_Panel_Internal.cpp)
