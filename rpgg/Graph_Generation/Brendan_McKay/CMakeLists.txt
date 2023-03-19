cmake_minimum_required(VERSION 3.18)
project(graphconverter)

find_package(igraph REQUIRED)

add_executable(graphconverter graphconverter.cpp graphconverter.h main.cpp)
target_link_libraries(graphconverter PUBLIC igraph::igraph)