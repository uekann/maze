cmake_minimum_required(VERSION 3.22)
project(maze)

set(CMAKE_CXX_STANDARD 14)

add_executable(maze main.cpp maze.h maze.cpp direction.cpp direction.h make_maze_sample.cpp)
add_executable(make_maze make_maze_sample.cpp maze.h maze.cpp direction.cpp direction.h)