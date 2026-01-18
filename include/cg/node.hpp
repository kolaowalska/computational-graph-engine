#pragma once
#include "concepts.hpp"
#include "node_id.hpp"

#include <span>
#include <string>
#include <vector>

namespace cg {

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

        // TODO: evaluate_from_cache
    };

    template<Numeric T>
    class ConstantNode final : public Node<T> {
    public:
        explicit ConstantNode(T v) : value_(v) {}

        std::string_view kind() const noexcept override { return "const"; }
        std::span<const NodeID> inputs() const noexcept override { return {}; }

        T value() const noexcept { return value_; }

    private:
        T value_;
    };

    template<Numeric T>
    class InputNode final : public Node<T> {
    public:
        explicit InputNode(std::string name) : name_(std::move(name)) {}

        std::string_view kind() const noexcept override { return "input"; }
        std::span<const NodeID> inputs() const noexcept override { return {}; }

        const std::string& name() const noexcept {return name_; }

    private:
        std::string name_;
    };

    template<Numeric T, UnaryOperation<T> Op>
    class UnaryNode final : public Node<T> {
    public:
        UnaryNode(NodeID in, Op op = {}) : in_(in), op_(std::move(op)) {}

        std::string_view kind() const noexcept override { return "unary"; }
        std::span<const NodeID> inputs() const noexcept override {
            return std::span<const NodeID>(&in_, 1);
        }

        NodeID input() const noexcept { return in_; }
        const Op& op() const noexcept { return op_; }

    private:
        NodeID in_;
        Op op_;
    };

    template<Numeric T, BinaryOperation<T> Op>
    class BinaryNode final : public Node<T> {
    public:
        BinaryNode(NodeID x, NodeID y, Op op = {}) : ins_{x, y}, op_(std::move(op)) {}

        std::string_view kind() const noexcept override { return "binary"; }
        std::span<const NodeID> inputs() const noexcept override {
            return std::span<const NodeID>(ins_.data(), ins_.size());
        }

        NodeID left() const noexcept { return ins_[0]; }
        NodeID right() const noexcept { return ins_[1]; }
        const Op& op() const noexcept { return op_; }

    private:
        std::array<NodeID, 2> ins_{};
        Op op_;
    };

}