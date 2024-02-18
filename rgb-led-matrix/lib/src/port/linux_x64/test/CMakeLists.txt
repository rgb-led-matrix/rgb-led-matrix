add_executable(linux_x64-test
    main.cpp)

target_compile_options(linux_x64-test PRIVATE
    -Wall
    -Werror
    -O3
)

target_link_libraries(linux_x64-test
    linux_x64)
