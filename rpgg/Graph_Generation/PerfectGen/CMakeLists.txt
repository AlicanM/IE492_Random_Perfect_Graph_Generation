add_library(perfectgen perfectgen.cpp)
target_link_libraries(perfectgen PUBLIC igraph::igraph)

add_executable(try_perfectgen try_perfectgen.cpp)
target_link_libraries(try_perfectgen PUBLIC igraph::igraph)
target_link_libraries(try_perfectgen PRIVATE perfectgen)
target_link_libraries(try_perfectgen PRIVATE graphconverter)