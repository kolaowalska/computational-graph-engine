#pragma once
#include "../graph.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <concepts>
#include <stdexcept>

namespace cg {
    template <typename T>
    using Context = std::unordered_map<std::string, T>;
}

namespace cg::eval {

    // defining the shared interface explicitly with a concept
    template <typename P, typename T>
    concept EvaluationPolicy =
        requires (const P& p, const Graph<T>& G, NodeID root, const Context<T>& ctx)
    {
        { p(G, root, ctx) } -> std::convertible_to<T>;
    };

    struct NaiveEvaluator {
        template<cg::Numeric T>
        T operator()(const cg::Graph<T>& G, cg::NodeID root, const cg::Context<T>& ctx) const {
            std::vector<T> values(G.size()); // storage for computed values
            auto order = G.topological_sort(); // get safe execution order

            // evaluate nodes
            for (auto id : order) {
                const auto& node = G.node(id);
                // special handling for input nodes: fetch from context
                if (node.kind() == "input") {
                    // static_cast is safe here because we checked kind()
                    const auto& input = static_cast<const cg::InputNode<T>&>(node);
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


    struct LazyEvaluator {
        template <cg::Numeric T>
        T operator()(const cg::Graph<T>& G, cg::NodeID root, const cg::Context<T>& ctx) const {
            std::vector<T> values(G.size()); // storage for computed values
            std::vector<bool> computed(G.size(), false);

            return recursive_evaluate(root, G, values, computed, ctx);
        }

    private:
        template <cg::Numeric T>
        T recursive_evaluate(cg::NodeID id, const cg::Graph<T>& G,
                         std::vector<T>& values, std::vector<bool>& computed,
                         const cg::Context<T>& ctx) const {

            size_t idx = id.index();

            if (computed[idx]) return values[idx];

            const auto& node = G.node(id);

            for (auto dependency : node.inputs()) {
                recursive_evaluate(dependency, G, values, computed, ctx);
            }

            if (node.kind() == "input") {
                const auto& input = static_cast<const cg::InputNode<T>&>(node);
                auto it = ctx.find(input.name());
                if (it == ctx.end()) {
                    throw std::runtime_error("missing value for input variable: " + input.name());
                }
                values[idx] = it->second;
            } else {
                values[idx] = node.evaluate_from_cache(values);
            }
            computed[idx] = true;
            return values[idx];
        }
    };
}