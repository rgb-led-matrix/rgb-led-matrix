add_library(rock_pi_s SHARED
    gpio/gpio.cpp
    pin-mapper/external/RP2040/RP2040_Multiplexed_PMP_Pins.cpp
    pin-mapper/HUB75/HUB75_Pins.cpp
    pin-mapper/PinMapping.cpp
    ../../led-matrix.cpp
    ../../framecanvas.cpp)

set(CMAKE_C_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

target_compile_definitions(rock_pi_s PUBLIC
    GPIO_BITS_T_DEFINE=uint32_t)

target_link_libraries(rock_pi_s
    pixel-mapper
    multiplex-mapper
    framebuffer
    pin-pulser)

add_executable(rock_pi_s-helper
    helper.cpp
    gpio/gpio.cpp)
    
target_compile_definitions(rock_pi_s-helper PUBLIC
    GPIO_BITS_T_DEFINE=uint32_t)
    
target_link_libraries(rock_pi_s-helper
    pthread)
