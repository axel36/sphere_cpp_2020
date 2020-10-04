cmake_minimum_required(VERSION 3.17)
project(hw_1_process)

set(CMAKE_CXX_STANDARD 17)

add_library(hw_1_process SHARED src/Process.cpp)

target_include_directories(hw_1_process PUBLIC include)
