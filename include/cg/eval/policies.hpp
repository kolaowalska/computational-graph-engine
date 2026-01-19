#pragma once
#include "../graph.hpp"
#include <unordered_map>
#include <string>

namespace cg {
    template <typename T>
    using Context = std::unordered_map<std::string, T>;
}

namespace cg::eval {

    struct NaiveEvaluator {
        template<cg::Numeric T>
        T operator()(const cg::Graph<T>& G, cg::NodeID root, const cg::Context<T>& ctx) const {
            std::vector<T> values(G.size());

            // TODO: implement seeding inputs

            return values[root.index()];
        }
    };

    struct MemoizedEvaluator : NaiveEvaluator {};

}