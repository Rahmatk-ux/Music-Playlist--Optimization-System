cmake_minimum_required(VERSION 3.10)
project(MusicPlaylistOptimizer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Graph.cpp
    src/Dijkstra.cpp
    src/TSP.cpp
    src/MST.cpp
)

# Executable
add_executable(playlist_optimizer ${SOURCES})

# Include directories
target_include_directories(playlist_optimizer PRIVATE src)