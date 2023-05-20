add_executable(cl_odd Clean_Odd_gen.cpp)
target_link_libraries(cl_odd PUBLIC igraph::igraph)
target_link_libraries(cl_odd PRIVATE graphconverter)