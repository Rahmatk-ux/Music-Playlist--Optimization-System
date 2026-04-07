cmake_minimum_required(VERSION 3.10)
project(MusicPlaylistOptimizer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/Graph.cpp
    src/Dijkstra.cpp
    src/TSP.cpp
    src/MST.cpp
)

add_executable(playlist_optimizer ${SOURCES})

target_include_directories(playlist_optimizer PRIVATE src)