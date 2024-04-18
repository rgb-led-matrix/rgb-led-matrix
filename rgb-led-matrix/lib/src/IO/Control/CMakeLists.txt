add_subdirectory(COM_Control)

add_library(Control INTERFACE)

target_link_libraries(Control INTERFACE
    COM_Control)
