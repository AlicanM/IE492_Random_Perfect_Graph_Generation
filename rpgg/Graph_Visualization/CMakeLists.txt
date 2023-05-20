add_executable(plotGraph main.cpp)
target_link_libraries(plotGraph PUBLIC igraph::igraph)
target_link_libraries(plotGraph PRIVATE graphconverter)