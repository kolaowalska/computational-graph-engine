#pragma once
#include "../graph.hpp"
#include <vector>

namespace cg::opt {

    template<cg::Numeric T>
    class ConstantFolding {
    public:
        void run(cg::Graph<T>& G, cg::NodeID root) {
            auto order = G.topological_sort();

            std::vector<bool> is_const(G.size(), false);
            std::vector<T> const_values(G.size());

            for (auto id : order) {
                const auto& node = G.node(id);
                size_t idx = id.index();

                if (node.kind() == "const") {
                    is_const[idx] = true;
                    const_values[idx] = node.evaluate_from_cache(const_values);
                } else if (node.kind() == "input") {
                    is_const[idx] = false;
                } else {
                    bool all_inputs_const = true;
                    for (auto dep : node.inputs()) {
                        if (!is_const[dep.index()]) {
                            all_inputs_const = false;
                            break;
                        }
                    }

                    if (all_inputs_const) {
                        is_const[idx] = true;
                        const_values[idx] = node.evaluate_from_cache(const_values);
                        G.replace(id, std::make_unique<ConstantNode<T>>(const_values[idx]));
                    } else {
                        is_const[idx] = false;
                    }
                }
            }
        }
    };
}