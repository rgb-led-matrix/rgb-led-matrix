add_library(BCM INTERFACE)

target_sources(BCM INTERFACE
    BCM.cpp
    ../framebuffer.cc)