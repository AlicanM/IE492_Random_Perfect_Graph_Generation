cmake_minimum_required(VERSION 3.18)
set (CMAKE_CXX_STANDARD 17)

project(rpgg)

find_package(igraph REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/Graph_Recognition/Oylum)
include_directories(${CMAKE_SOURCE_DIR}/Graph_Generation/Brendan_McKay)
include_directories(${CMAKE_SOURCE_DIR}/Graph_Generation/C5_Free)
include_directories(${CMAKE_SOURCE_DIR}/Graph_Generation/PerfectGen)

link_directories(${CMAKE_SOURCE_DIR}/Graph_Recognition/Oylum)
link_directories(${CMAKE_SOURCE_DIR}/Graph_Generation/Brendan_McKay)
link_directories(${CMAKE_SOURCE_DIR}/Graph_Generation/C5_Free)
link_directories(${CMAKE_SOURCE_DIR}/Graph_Generation/PerfectGen)

add_subdirectory(Graph_Recognition/Oylum)
add_subdirectory(Graph_Generation/Brendan_McKay)
add_subdirectory(Graph_Generation/C5_Free)
#add_subdirectory(Graph_Generation/CleanOddCycle_Generation)
add_subdirectory(Graph_Generation/PerfectGen)
add_subdirectory(Graph_Generation/Markov_Chain_BFS)

add_subdirectory(Tests)