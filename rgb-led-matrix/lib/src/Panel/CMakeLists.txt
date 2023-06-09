add_subdirectory(LEDPanel)

add_library(Panel INTERFACE)

target_link_libraries(Panel INTERFACE
    LEDPanel)