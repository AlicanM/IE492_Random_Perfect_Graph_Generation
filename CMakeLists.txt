cmake_minimum_required(VERSION 3.18)
project(igraph_rpgg)

find_package(igraph REQUIRED)

add_executable(igraph_rpgg igraph_rpgg.cpp)
target_link_libraries(igraph_rpgg PUBLIC igraph::igraph)