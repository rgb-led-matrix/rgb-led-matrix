add_library(Exception INTERFACE)

target_sources(Exception INTERFACE
    String_Exception.cpp
    Null_Pointer.cpp)