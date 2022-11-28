cmake_minimum_required(VERSION 3.13)

project(mod C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(firmware)