cmake_minimum_required(VERSION 3.17)
project(hw_5_net)

set(CMAKE_CXX_STANDARD 17)

add_library(hw_5_net STATIC src/service.cpp src/epoll.cpp)
target_include_directories(hw_5_net PUBLIC include)

target_link_libraries(hw_5_net hw_1_descriptor)
target_link_libraries(hw_5_net hw_2_logger)
target_link_libraries(hw_5_net hw_3_tcp)

add_executable(test_hw_5 tests/test.cpp)
target_link_libraries(test_hw_5 hw_5_net)
#
#add_executable(test_server_hw_3 tests/test_server.cpp)
#target_link_libraries(test_server_hw_3 hw_3_tcp)
