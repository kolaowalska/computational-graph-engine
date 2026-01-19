#pragma once
#include <concepts>
#include "../graph.hpp"

namespace cg::opt {

    template<typename Pass, typename T>
    concept GraphPass =
        requires(Pass pass, cg::Graph<T>& G, cg::NodeID root)
    {
        pass.run(G, root) -> std::same_as<void>;
    };

}