add_library(graphstatistics graphstatistics.cpp)
target_link_libraries(graphstatistics PUBLIC igraph::igraph)

target_link_libraries(graphstatistics PUBLIC igraph::igraph)
target_link_libraries(graphstatistics PRIVATE graphconverter)