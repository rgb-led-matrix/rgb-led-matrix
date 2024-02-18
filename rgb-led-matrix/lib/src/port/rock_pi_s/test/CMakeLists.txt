add_executable(rock_pi_s-test
    main.cpp)

target_compile_options(rock_pi_s-test PRIVATE
    -Wall
    -Werror
    -O3
)

target_link_libraries(rock_pi_s-test
    rock_pi_s)
