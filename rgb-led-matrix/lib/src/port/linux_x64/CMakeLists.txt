add_library(linux_x64 SHARED
    ../../RGBMatrix.cpp)

target_include_directories(linux_x64 PUBLIC
    ../../../../include/
    ../../../include/
    ../../../../../external/FTDI/FTDI_D2XX/Linux/x64/)

target_link_directories(linux_x64 PUBLIC
    ../../../../../external/FTDI/FTDI_D2XX/Linux/x64/build)

target_compile_options(linux_x64 PRIVATE
    -Wall
    -Werror
    -O3
)

target_link_libraries(linux_x64
    Exception
    ThreadPool
    Mapper
    CFG
    IO
    Panel
    Frame
    ftd2xx
    pthread)

add_subdirectory(test)
