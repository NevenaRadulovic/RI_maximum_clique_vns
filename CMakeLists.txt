cmake_minimum_required(VERSION 3.16.)
project(RI_MAXIMUM_CLIQUE VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2")

add_library(
    Graph
    Graph.hpp
    Graph.cpp
)


add_executable(vns vns.cpp)
add_executable(bruteforce bruteforce.cpp)

target_link_libraries(vns PRIVATE Graph)
target_link_libraries(bruteforce PRIVATE Graph)


