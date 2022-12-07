add_executable(rpi
    gpio/gpio.cpp
    thread/thread.cpp
    pin-pulser/PinPulser.cpp
    pin-pulser/PICO/PICO.cpp
    pin-mapper/external/RP2040/RP2040_Multiplexed_Dual_UART_Pins.cpp
    pin-mapper/HUB75/HUB75_Pins.cpp
    pin-mapper/PinMapping.cpp
    ../../led-matrix.cpp
    ../../framecanvas.cpp
    main.cpp)

set(CMAKE_C_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/pi/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

target_compile_definitions(rpi PUBLIC
    GPIO_BITS_T_DEFINE=uint32_t)

target_link_libraries(rpi
    pixel-mapper
    multiplex-mapper
    framebuffer
    pthread)
