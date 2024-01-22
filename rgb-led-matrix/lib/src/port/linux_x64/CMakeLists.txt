add_library(linux_x64 SHARED
    ../../RGBMatrix.cpp)

target_include_directories(linux_x64 PUBLIC
    ../../../../include/
    ../../../include/)

target_link_libraries(linux_x64
    Framebuffer
    CFG
    IO
    Panel
    pthread)

add_subdirectory(test)
