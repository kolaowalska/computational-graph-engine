#pragma once
#include "concepts.hpp"
#include "node.hpp"

#include <memory>
#include <queue>


namespace cg {

    // owns nodes and provides building & traversal utilities

    template<Numeric T>
    class Graph {
    public:
        using value_type = T;

        std::size_t size() const { return nodes_.size(); }

        const Node<T>& node(NodeID id) const {
            return *nodes_.at(id.index());
        }

        Node<T>& node(NodeID id) {
            return *nodes_.at(id.index());
        }

        NodeID add(std::unique_ptr<Node<T>> node) {
            nodes_.push_back(std::move(node));
            return NodeID{nodes_.size() - 1};
        }

        void replace(NodeID id, std::unique_ptr<Node<T>> node) {
            nodes_.at(id.index()) = std::move(node);
        }

        NodeID constant(T v) {
            return add(std::make_unique<ConstantNode<T>>(v));
        }

        NodeID input(std::string name) {
            return add(std::make_unique<InputNode<T>>(std::move(name)));
        }


    private:
        std::vector<std::unique_ptr<Node<T>>> nodes_;
    };
}