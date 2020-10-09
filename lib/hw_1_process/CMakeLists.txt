cmake_minimum_required(VERSION 3.17)
project(hw_1_process)

set(CMAKE_CXX_STANDARD 17)

add_library(hw_1_process STATIC src/Process.cpp)
target_include_directories(hw_1_process PUBLIC include)

add_executable(test_hw_1_process tests/hw1_test.cpp src/Process.cpp)
target_include_directories(test_hw_1_process PRIVATE include)
