cmake_minimum_required(VERSION 3.22)
project(tetris_player)

set(CMAKE_CXX_STANDARD 17)

add_executable(tetris_player src/Main.cpp)

target_include_directories(tetris_player PUBLIC ./src/include)