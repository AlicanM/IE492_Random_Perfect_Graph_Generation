add_library(oylum oylum.cpp)
target_link_libraries(oylum PUBLIC igraph::igraph)

add_executable(converter converter.cpp)
target_link_libraries(converter PUBLIC igraph::igraph)
target_link_libraries(converter PRIVATE graphconverter)
