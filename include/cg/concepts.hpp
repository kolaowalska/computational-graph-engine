#pragma once
#include <concepts>

namespace cg {

    // Numeric<T>: a numeric-like requirement for graph value types

    template <typename T>
    concept Numeric =
        requires (T a, T b)
    {
        { a + b } -> std::convertible_to<T>;
        { a - b } -> std::convertible_to<T>;
        { a * b } -> std::convertible_to<T>;
        { a / b } -> std::convertible_to<T>;
    };

    // UnaryOperation<Op, T>: operation functor must be callable as T f(T)

    template <typename Op, typename T>
    concept UnaryOperation =
        requires(Op o, T x)
    {
        { o(x) } -> std::same_as<T>;
    };

    // BinaryOperation<Op, T>: operation functor must be callable as T f(T, T)

    template <typename Op, typename T>
    concept BinaryOperation =
        requires (Op o, T x, T y)
    {
        { o(x, y) } -> std::same_as<T>;
    };

} // namespace cg