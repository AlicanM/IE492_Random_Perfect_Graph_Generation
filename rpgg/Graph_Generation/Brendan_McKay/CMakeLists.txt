add_library(graphconverter graphconverter.cpp)
target_link_libraries(graphconverter PUBLIC igraph::igraph)

add_executable(try_graphconverter main.cpp)
target_link_libraries(try_graphconverter PUBLIC igraph::igraph)
target_link_libraries(try_graphconverter PRIVATE graphconverter)

