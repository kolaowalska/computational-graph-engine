#pragma once
#include "concepts.hpp"
#include "node_id.hpp"

#include <span>
#include <string>
#include <vector>
#include <functional>
#include <typeinfo>
#include <sstream>
#include <iomanip>

namespace cg {

    inline void hash_combine(std::size_t& seed, std::size_t value) {
        seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // a runtime-polymorphic base for all node types
    template<Numeric T>
    class Node {
    public:
        using value_type = T;
        virtual ~Node() = default;

        // readable name for debugging
        virtual std::string_view kind() const noexcept = 0;

        // returns a list of dependency node IDs
        virtual std::span<const NodeID> inputs() const noexcept = 0;

        virtual std::size_t hash() const noexcept = 0;
        virtual bool is_equivalent(const Node& other) const noexcept = 0;

        // uses precomputed values[child.index()] to avoid recursion when computing its own value
        virtual T evaluate_from_cache(std::span<const T> values) const = 0;

        virtual std::string label() const noexcept = 0;
    };

    template<Numeric T>
    class ConstantNode final : public Node<T> {
    public:
        explicit ConstantNode(T v) : value_(v) {}

        std::string_view kind() const noexcept override { return "const"; }
        std::span<const NodeID> inputs() const noexcept override { return {}; }
        T evaluate_from_cache(std::span<const T> values) const override { return value_; }

        // constants are equal if values are equal
        std::size_t hash() const noexcept override {
            return std::hash<T>{}(value_);
        }

        bool is_equivalent(const Node<T>& other) const noexcept override {
            if (auto* c = dynamic_cast<const ConstantNode*>(&other)) {
                return value_ == c->value_;
            }
            return false;
        }

        T value() const noexcept { return value_; }

        std::string label() const noexcept override {
            std::ostringstream oss;
            oss << value_;
            return oss.str();
        }

    private:
        T value_;
    };

    template<Numeric T>
    class InputNode final : public Node<T> {
    public:
        explicit InputNode(std::string name) : name_(std::move(name)) {}

        std::string_view kind() const noexcept override { return "input"; }
        std::span<const NodeID> inputs() const noexcept override { return {}; }

        T evaluate_from_cache(std::span<const T> values) const override {
            throw std::logic_error("not implemented");
        }

        // inputs are equal if names are equal
        std::size_t hash() const noexcept override {
            return std::hash<std::string>{}(name_);
        }
        bool is_equivalent(const Node<T>& other) const noexcept override {
            if (auto* in = dynamic_cast<const InputNode*>(&other)) {
                return name_ == in->name_;
            }
            return false;
        }

        const std::string& name() const noexcept {return name_; }

        std::string label() const noexcept override {
            return name_;
        }

    private:
        std::string name_;
    };

    template<Numeric T, UnaryOperation<T> O>
    class UnaryNode final : public Node<T> {
    public:
        UnaryNode(NodeID in, O o = {}) : in_(in), o_(std::move(o)) {}

        std::string_view kind() const noexcept override { return "unary"; }

        std::span<const NodeID> inputs() const noexcept override {
            return std::span(&in_, 1);
        }

        T evaluate_from_cache(std::span<const T> values) const override {
            return o_(values[in_.index()]);
        }

        // same input + same operation type = same node
        std::size_t hash() const noexcept override {
            std::size_t h = 0;
            hash_combine(h, in_.index());
            hash_combine(h, typeid(O).hash_code());
            return h;
        }

        bool is_equivalent(const Node<T>& other) const noexcept override {
            if (auto* u = dynamic_cast<const UnaryNode*>(&other)) {
                return in_ == u->in_;
            }
            return false;
        }

        NodeID input() const noexcept { return in_; }
        const O& o() const noexcept { return o_; }

        std::string label() const noexcept override {
            return std::string(O::symbol);
        }

    private:
        NodeID in_;
        O o_;
    };

    template<Numeric T, BinaryOperation<T> O>
    class BinaryNode final : public Node<T> {
    public:
        BinaryNode(NodeID x, NodeID y, O o = {}) : ins_{x, y}, o_(std::move(o)) {}

        std::string_view kind() const noexcept override { return "binary"; }

        std::span<const NodeID> inputs() const noexcept override {
            return std::span(ins_.data(), ins_.size());
        }

        T evaluate_from_cache(std::span<const T> values) const override {
            // const auto a = values[ins_[0].index()];
            // const auto b = values[ins_[1].index()];
            // return o(a, b);
            return o_(values[ins_[0].index()], values[ins_[1].index()]);
        }

        // same input + same operation type = same node
        std::size_t hash() const noexcept override {
            std::size_t h = 0;
            hash_combine(h, ins_[0].index());
            hash_combine(h, ins_[1].index());
            hash_combine(h, typeid(O).hash_code());
            return h;
        }
        bool is_equivalent(const Node<T>& other) const noexcept override {
            if (auto* b = dynamic_cast<const BinaryNode*>(&other)) {
                return ins_ == b->ins_;
            }
            return false;
        }

        NodeID left() const noexcept { return ins_[0]; }
        NodeID right() const noexcept { return ins_[1]; }
        const O& o() const noexcept { return o_; }

        std::string label() const noexcept override {
            return std::string(O::symbol);
        }

    private:
        std::array<NodeID, 2> ins_{};
        O o_;
    };

} // namespace cg