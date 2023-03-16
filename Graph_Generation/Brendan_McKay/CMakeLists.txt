cmake_minimum_required(VERSION 3.18)
project(graph6_to_igraph)

find_package(igraph REQUIRED)

add_executable(graph6_to_igraph graph6_to_igraph.cpp)
target_link_libraries(graph6_to_igraph PUBLIC igraph::igraph)