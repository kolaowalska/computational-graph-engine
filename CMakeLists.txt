cmake_minimum_required(VERSION 4.1)
project(computational-graph-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cg INTERFACE)

target_include_directories(cg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(cg INTERFACE
        include/cg/node.hpp
        include/cg/node_id.hpp
        include/cg/concepts.hpp
        include/cg/graph.hpp
        include/cg/expression.hpp
        include/cg/ops.hpp
        include/cg/eval/policies.hpp
        include/cg/eval/evaluator.hpp
        include/cg/opt/constant_folding.hpp
)

add_executable(computational-graph-engine src/main.cpp)

target_link_libraries(computational-graph-engine PRIVATE cg)
