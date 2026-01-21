#pragma once
#include "concepts.hpp"
#include <cmath>


namespace cg::ops {

    struct Add {
        static constexpr auto symbol = "+";

        template<cg::Numeric T>
        T operator()(T x, T y) const { return x + y; }
    };

    struct Sub {
        static constexpr auto symbol = "-";

        template<cg::Numeric T>
        T operator()(T x, T y) const { return x - y; }
    };

    struct Mul {
        static constexpr auto symbol = "*";

        template<cg::Numeric T>
        T operator()(T x, T y) const { return x * y; }
    };

    struct Div {
        static constexpr auto symbol = "/";

        template<cg::Numeric T>
        T operator()(T x, T y) const { return x / y; }
    };

    struct Neg {
        static constexpr auto symbol = "~";

        template<cg::Numeric T>
        T operator()(T x) const { return -x; }
    };

    struct Sin {
        static constexpr auto symbol = "sin";

        template<cg::Numeric T>
        T operator()(T x) const { using std::sin; return sin(x); }
    };

    struct Cos {
        static constexpr auto symbol = "cos";

        template<cg::Numeric T>
        T operator()(T x) const { using std::cos; return cos(x); }
    };

    struct Exp {
        static constexpr auto symbol = "exp";

        template<cg::Numeric T>
        T operator()(T x) const { using std::exp; return exp(x); }
    };

    struct Log {
        static constexpr auto symbol = "log";

        template<cg::Numeric T>
        T operator()(T x) const { using std::log; return log(x); }
    };

    struct Pow {
        static constexpr auto symbol = "pow";

        template<cg::Numeric T>
        T operator()(T base, T exponent) const { using std::pow; return pow(base, exponent); }
    };

    struct Sqrt {
        static constexpr auto symbol = "sqrt";

        template<cg::Numeric T>
        T operator()(T x) const { using std::sqrt; return sqrt(x); }
    };



}