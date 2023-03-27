add_executable(mc_bfs mc_bfs.cpp)
target_link_libraries(mc_bfs PUBLIC igraph::igraph)
target_link_libraries(mc_bfs PRIVATE c5_free_gen)