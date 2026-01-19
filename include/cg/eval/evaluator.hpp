#pragma once
#include "policies.hpp"

namespace cg {

    template<Numeric T, typename Policy>
    class Evaluator {
    public:
        explicit Evaluator(Policy policy = {}) : policy_(std::move(policy)) {}

        T evaluate(const Graph<T>& G, NodeID root, const Context<T>& ctx) const {
            return policy_(G, root, ctx);
        }

    private:
        Policy policy_;

    };
}