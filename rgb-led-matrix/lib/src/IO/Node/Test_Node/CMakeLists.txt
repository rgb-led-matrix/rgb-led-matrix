add_library(Test_Node INTERFACE)

target_sources(Test_Node INTERFACE
    Test_Node.cpp)

target_link_libraries(Test_Node INTERFACE
    Node)
