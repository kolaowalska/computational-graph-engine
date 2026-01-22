cmake_minimum_required(VERSION 3.20)
project(computational_graph_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(computational_graph_engine src/main.cpp)

add_library(cg INTERFACE)

target_include_directories(cg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(cg INTERFACE
        include/cg/node.hpp
        include/cg/node_id.hpp
        include/cg/concepts.hpp
        include/cg/graph.hpp
        include/cg/expression.hpp
        include/cg/ops.hpp
        include/cg/dual.hpp
        include/cg/eval/policies.hpp
        include/cg/eval/evaluator.hpp
        include/cg/opt/constant_folding.hpp
        include/cg/opt/pass.hpp
        include/cg/viz/dot.hpp
        include/cg/viz/visualizer.hpp
        tests/test_basic.cpp)

target_include_directories(cg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(computational_graph_engine PRIVATE cg)


enable_testing()
add_executable(cg_tests tests/test_basic.cpp)
target_link_libraries(cg_tests PRIVATE cg)

add_test(NAME basic_graph_test COMMAND cg_tests)
