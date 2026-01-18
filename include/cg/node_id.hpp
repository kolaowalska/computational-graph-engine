#pragma once
#include <cstddef>
#include <compare>

namespace cg {

    // a strongly-typed handle for nodes to avoid passing raw indices and pointers around
    struct NodeID {
        std::size_t idx{0};

        constexpr std::size_t index() const noexcept { return idx; }

        friend constexpr bool operator==(NodeID, NodeID) = default;
        friend constexpr auto operator<=>(NodeID, NodeID) = default;
    };
}