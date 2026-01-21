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
            auto h = node->hash();

            auto range = cache_.equal_range(h);
            for (auto it = range.first; it != range.second; ++it) {
                NodeID existing = it->second;
                if (nodes_[existing.index()]->is_equivalent(*node)) {
                    return existing;
                }
            }

            nodes_.push_back(std::move(node));
            NodeID new_id{nodes_.size() - 1};
            // TODO: cache_.insert({h, new_id});
            cache_.insert(std::make_pair(h, new_id));
            return new_id;
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

        // kahn
        std::vector<NodeID> topological_sort() const {
            std::vector<size_t> indegree(nodes_.size(), 0);
            std::vector<std::vector<size_t>> adj(nodes_.size());

            for (size_t i = 0; i < nodes_.size(); ++i) {
                for (const auto& dep : nodes_[i]->inputs()) {
                    adj[dep.index()].push_back(i);
                    ++indegree[i];
                }
            }

            std::queue<size_t> q;
            for (size_t i = 0; i < nodes_.size(); ++i) {
                if (indegree[i] == 0) {
                    q.push(i);
                }
            }

            std::vector<NodeID> sorted;
            sorted.reserve(nodes_.size());

            while (!q.empty()) {
                size_t u = q.front();
                q.pop();
                sorted.push_back(NodeID{u});

                for (size_t v : adj[u]) {
                    --indegree[v];
                    if (indegree[v] == 0) {
                        q.push(v);
                    }
                }
            }

            if (sorted.size() != nodes_.size()) {
                throw std::runtime_error("graph contains a cycle >:(");
            }

            return sorted;
        }

    private:
        std::vector<std::unique_ptr<Node<T>>> nodes_;
        std::unordered_multimap<std::size_t, NodeID> cache_;
    };

} // namespace cg