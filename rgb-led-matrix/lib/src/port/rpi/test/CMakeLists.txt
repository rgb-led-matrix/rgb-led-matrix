add_executable(rpi-test
    main.cpp)

target_compile_options(rpi-test PRIVATE
    -Wall
    -Werror
    -O3
)

target_link_libraries(rpi-test
    rpi)
