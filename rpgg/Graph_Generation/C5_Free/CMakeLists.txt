add_library(c5_free_gen c5_free_gen.cpp)
target_link_libraries(c5_free_gen PUBLIC igraph::igraph)


add_executable(try_c5_free_gen try_c5_free_gen.cpp)
target_link_libraries(try_c5_free_gen PUBLIC igraph::igraph)
target_link_libraries(try_c5_free_gen PRIVATE c5_free_gen)

