cmake_minimum_required(VERSION 3.20)
project(computational_graph_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cg INTERFACE)
target_include_directories(cg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(cg_main src/main.cpp)
target_link_libraries(cg_main PRIVATE cg)

enable_testing()
add_executable(cg_tests tests/test_basic.cpp)
target_link_libraries(cg_tests PRIVATE cg)

add_test(NAME basic_graph_test COMMAND cg_tests)