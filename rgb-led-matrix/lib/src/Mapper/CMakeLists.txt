add_library(Mapper INTERFACE)

target_sources(Mapper INTERFACE
    Multiplex/Multiplex.cpp
    Multiplex/Outdoor/Outdoor.cpp)
