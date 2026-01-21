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
            std::vector<T> values(G.size()); // storage for computed values
            auto order = G.topological_sort(); // get safe execution order

            // evaluate nodes
            for (NodeID id : order) {
                const auto& node = G.node(id);
                // special handling for input nodes: fetch from context
                if (node.kind() == "input") {
                    // static_cast is safe here because we checked kind()
                    const auto& input = static_cast<const InputNode<T>&>(node);
                    auto it = ctx.find(input.name());

                    if (it == ctx.end()) {
                        throw std::runtime_error("missing value for input variable: " + input.name());
                    }
                    values[id.index()] = it->second;
                } else {
                    // all other nodes can self-evaluate using the cache
                    values[id.index()] = node.evaluate_from_cache(values);
                }
            }
            return values[root.index()];
        }
    };
}