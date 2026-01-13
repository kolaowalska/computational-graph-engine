cmake_minimum_required(VERSION 4.1)
project(computational_graph_framework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cg INTERFACE
        include/cg/node.hpp
        include/cg/node_id.hpp
        include/cg/concepts.hpp
        include/cg/graph.hpp
        include/cg/expression.hpp
        include/cg/ops.hpp
        include/cg/eval/policies.hpp
        include/cg/eval/evaluator.hpp
        include/cg/opt/constant_folding.hpp
        tests/test_basic.cpp)
target_include_directories(cg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(computational_graph_framework src/main.cpp)
