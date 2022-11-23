cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(mod C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(mod main.cpp OE_delay.S port.cpp)

add_subdirectory(src/Multiplex)

include_directories(. include)
        
target_compile_definitions(mod PUBLIC
	PICO_HEAP_SIZE=65536
	PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
)

target_compile_options(mod PUBLIC
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    -fno-exceptions 
    -fno-check-new 
    $<$<COMPILE_LANGUAGE:CXX>:-fno-enforce-eh-specs>
    -g 
    -ffunction-sections 
    -fdata-sections 
    -O3
    -funroll-loops 
    -Werror 
    -Wall)

pico_set_binary_type(mod copy_to_ram)

# create map/bin/hex file etc.
pico_add_extra_outputs(mod)

# enable usb output, disable uart output
pico_enable_stdio_usb(mod 1)
pico_enable_stdio_uart(mod 0)

# create disassembly with source
function(pico_add_dis_output2 TARGET)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -S $<TARGET_FILE:${TARGET}> >$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.dis2)
endfunction()
pico_add_dis_output2(mod)

# pull in dependencies
target_link_libraries(mod 
	pico_stdlib
	pico_multicore)
