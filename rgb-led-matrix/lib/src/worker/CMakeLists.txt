add_library(worker INTERFACE)

target_sources(worker INTERFACE
    UpdateThread.cpp
    BackgroundThread.cpp)