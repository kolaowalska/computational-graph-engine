#pragma once
#include "graph.hpp"
#include "ops.hpp"

#include <cassert>
#include <numbers>

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
    Expression<T> pi(Graph<T>& G) {
        return constant(G, std::numbers::pi_v<T>);
    }

    template<Numeric T>
    Expression<T> e(Graph<T>& G) {
        return constant(G, std::numbers::e_v<T>);
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
        assert(&a.graph() == &b.graph() && "cannot combine expressions from different graphs :(");

        auto& G = a.graph();
        auto id = G.add(std::make_unique<BinaryNode<T, O>>(a.root(), b.root(), std::move(operation)));
        return Expression<T>(&G, id);
    }

    // --------- EXPRESSION <-> EXPRESSION OPERATORS ---------

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
    Expression<T> pow(Expression<T> base, Expression<T> exponent) {
        return binary<T>(base, exponent, ops::Pow{});
    }

    // --------- EXPRESSION OPERATORS ---------

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

    template<Numeric T>
    Expression<T> exp(Expression<T> a) {
        return unary<T>(a, ops::Exp{});
    }

    template<Numeric T>
    Expression<T> log(Expression<T> a) {
        return unary<T>(a, ops::Log{});
    }

    template<Numeric T>
    Expression<T> sqrt(Expression<T> a) {
        return unary<T>(a, ops::Sqrt{});
    }

    // --------- EXPRESSION <-> SCALAR OPERATORS ---------

    template<Numeric T>
    Expression<T> operator+(Expression<T> a, T scalar) {
        return a + constant(a.graph(), scalar);
    }

    template<Numeric T>
    Expression<T> operator+(T scalar, Expression<T> a) {
        return constant(a.graph(), scalar) + a;
    }

    template<Numeric T>
    Expression<T> operator-(Expression<T> a, T scalar) {
        return a - constant(a.graph(), scalar);
    }

    template<Numeric T>
    Expression<T> operator-(T scalar, Expression<T> a) {
        return constant(a.graph(), scalar) - a;
    }

    template<Numeric T>
    Expression<T> operator*(Expression<T> a, T scalar) {
        return a * constant(a.graph(), scalar);
    }

    template<Numeric T>
    Expression<T> operator*(T scalar, Expression<T> a) {
        return constant(a.graph(), scalar) * a;
    }

    template<Numeric T>
    Expression<T> operator/(Expression<T> a, T scalar) {
        return a / constant(a.graph(), scalar);
    }

    template<Numeric T>
    Expression<T> operator/(T scalar, Expression<T> a) {
        return constant(a.graph(), scalar) / a;
    }

    template<Numeric T>
    Expression<T> pow(Expression<T> base, T exponent) {
        return binary<T>(base, constant(base.graph(), exponent), ops::Pow{});
    }

    template<Numeric T>
    Expression<T> pow(T base, Expression<T> exponent) {
        return binary<T>(constant(exponent.graph(), base), exponent, ops::Pow{});
    }

} // namespace cg