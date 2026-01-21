#pragma once
#include "../graph.hpp"
#include <ostream>
#include <string>

namespace cg::viz {

    template<Numeric T>
    void export_to_dot(const Graph<T>& G, std::ostream& out) {
        out << "digraph ComputationGraph {\n";
        out << "    rankdir=TB;\n";
        out << "    node [fontname=\"Courier New\", shape=box, style=filled, fillcolor=white];\n";

        for (size_t i = 0; i < G.size(); ++i) {
            NodeID id{i};
            const auto& node = G.node(id);

            std::string shape = "box";
            std::string color = "white";

            if (node.kind() == "input") {
                shape = "circle";
                color = "pink";
            } else if (node.kind() == "const") {
                shape = "box";
                color = "lightpink";
            } else {
                shape = "Mcircle";
                color = "peachpuff";
            }

            out << "    " << i << " ["
                << "label=\"" << node.label() << "\", "
                << "shape=" << shape << ", "
                << "fillcolor=" << color
                << "];\n";

            for (auto input_id : node.inputs()) {
                out << "    " << input_id.index() << " -> " << i << ";\n";
            }
        }

        out << "}\n";
    }

} // namespace cg::viz