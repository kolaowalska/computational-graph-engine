#pragma once
#include "graph.hpp"
#include "ops.hpp"

namespace cg {

    template<Numeric T>
    class Expression {
    public:
        Expression(Graph<T>* G, NodeID root) : G_(G), root_(root) {}

        Graph<T>& graph() const { return *G_; }
        NodeID root() const noexcept { return root_; }


    private:
        Graph<T>* G_;
        NodeID root_;
    };

    template<Numeric T>
    Expression<T> constant(Graph<T>& G, T value) {
        return Expression<T>(&G, G.constant(value));
    }

    template<Numeric T>
    Expression<T> input(Graph<T>& G, std::string name) {
        return Expression<T>(&G, G.input(std::move(name)));
    }

    template<Numeric T, UnaryOperation<T> O>
    Expression<T> unary(Expression<T> a, O operation = {}) {
        auto& G = a.graph();
        auto id = G.add(std::make_unique<UnaryNode<T, O>>(a.root(), std::move(operation)));
        return Expression<T>(&G, id);
    }

    template<Numeric T, BinaryOperation<T> O>
    Expression<T> binary(Expression<T> a, Expression<T> b, O operation = {}) {
        auto& G = a.graph();
        auto id = G.add(std::make_unique<BinaryNode<T, O>>(a.root(), b.root(), std::move(operation)));
        return Expression<T>(&G, id);
    }

    template<Numeric T>
    Expression<T> operator+(Expression<T> a, Expression<T> b) {
        return binary<T>(a, b, ops::Add{});
    }

    template<Numeric T>
    Expression<T> operator-(Expression<T> a, Expression<T> b) {
        return binary<T>(a, b, ops::Sub{});
    }

    template<Numeric T>
    Expression<T> operator*(Expression<T> a, Expression<T> b) {
        return binary<T>(a, b, ops::Mul{});
    }

    template<Numeric T>
    Expression<T> operator/(Expression<T> a, Expression<T> b) {
        return binary<T>(a, b, ops::Div{});
    }

    template<Numeric T>
    Expression<T> operator-(Expression<T> a) {
        return unary<T>(a, ops::Neg{});
    }

    template<Numeric T>
    Expression<T> sin(Expression<T> a) {
        return unary<T>(a, ops::Sin{});
    }

    template<Numeric T>
    Expression<T> cos(Expression<T> a) {
        return unary<T>(a, ops::Cos{});
    }

} // namespace cg