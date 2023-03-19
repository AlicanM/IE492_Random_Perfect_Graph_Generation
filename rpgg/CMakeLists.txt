cmake_minimum_required(VERSION 3.18)
project(rpgg)

find_package(igraph REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/Graph_Recognition/Oylum)

link_directories(${CMAKE_SOURCE_DIR}/Graph_Recognition/Oylum)

add_subdirectory(Graph_Recognition/Oylum)
add_subdirectory(Graph_Generation/Marcov_Chain_C5_Free)
add_subdirectory(Speed_Tests)

