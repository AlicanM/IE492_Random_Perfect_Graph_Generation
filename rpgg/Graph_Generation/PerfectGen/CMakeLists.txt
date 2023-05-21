add_library(perfectGen perfectgen.cpp)
target_link_libraries(perfectGen PUBLIC igraph::igraph)

add_executable(perfectgen main.cpp)
target_link_libraries(perfectgen PUBLIC igraph::igraph)
target_link_libraries(perfectgen PRIVATE perfectGen)
target_link_libraries(perfectgen PRIVATE graphconverter)
target_link_libraries(perfectgen PRIVATE graphstatistics)