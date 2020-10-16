cmake_minimum_required(VERSION 3.17)
project(main_proj)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_executable(main_proj src/main.cpp)
