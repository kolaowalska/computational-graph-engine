#pragma once
#include <cstddef>
#include <compare>

namespace cg {
    struct NodeID {
        std::size_t idx{0};

        constexpr std::size_t index() const noexcept { return idx; }

        friend constexpr bool operator==(NodeID, NodeID) = default;
        friend constexpr auto operator<=>(NodeID, NodeID) = default;
    };
}