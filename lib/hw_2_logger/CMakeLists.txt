cmake_minimum_required(VERSION 3.17)
project(hw_2_logger)

set(CMAKE_CXX_STANDARD 17)

add_library(hw_2_logger STATIC src/logger.cpp src/file_logger.cpp)
target_include_directories(hw_2_logger PUBLIC include)

add_executable(test_hw_2 tests/main_test.cpp)
target_link_libraries(test_hw_2 hw_2_logger)
