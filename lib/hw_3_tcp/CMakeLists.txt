cmake_minimum_required(VERSION 3.17)
project(hw_3_tcp)

set(CMAKE_CXX_STANDARD 17)

add_library(hw_3_tcp STATIC src/server.cpp src/connection.cpp)
target_include_directories(hw_3_tcp PUBLIC include)

target_link_libraries(hw_3_tcp hw_1_descriptor)
target_link_libraries(hw_3_tcp hw_2_logger)

add_executable(test_client_hw_3 tests/test_client.cpp)
target_link_libraries(test_client_hw_3 hw_3_tcp)

add_executable(test_server_hw_3 tests/test_server.cpp)
target_link_libraries(test_server_hw_3 hw_3_tcp)
