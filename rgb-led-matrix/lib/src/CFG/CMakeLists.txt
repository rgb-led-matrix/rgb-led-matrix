add_library(CFG INTERFACE)

target_sources(CFG INTERFACE
    CFG.cpp
    HUB75.cpp
    Types/DOTCorrect.cpp
    Types/GAMMA.cpp)