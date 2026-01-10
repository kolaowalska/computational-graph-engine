#pragma once
#include <concepts>
#include <type_traits>
#include <cmath>

namespace cg {

    template <typename T>
    concept Numeric =
        requires (T a, T b)
    {
        { a + b } -> std::convertible_to<T>;
        { a - b } -> std::convertible_to<T>;
        { a * b } -> std::convertible_to<T>;
        { a / b } -> std::convertible_to<T>;
    };

    template <typename Op, typename T>
    concept UnaryOperation =
        requires(Op o, T x)
    {
        { o(x) } -> std::same_as<T>;
    };

    template <typename Op, typename T>
    concept BinaryOperation =
        requires (Op o, T x, T y)
    {
        { o(x, y) } -> std::same_as<T>;
    };

}