#pragma once
#include "concepts.hpp"
#include <cmath>


namespace cg::ops {

    struct Add {
        template<cg::Numeric T>
        T operator()(T x, T y) const { return x + y; }
    };

    struct Sub {
        template<cg::Numeric T>
        T operator()(T x, T y) const { return x - y; }
    };

    struct Mul {
        template<cg::Numeric T>
        T operator()(T x, T y) const { return x * y; }
    };

    struct Div {
        template<cg::Numeric T>
        T operator()(T x, T y) const { return x / y; }
    };

    struct Neg {
        template<cg::Numeric T>
        T operator()(T x) const { return -x; }
    };

    struct Sin {
        template<cg::Numeric T>
        T operator()(T x) const { using std::sin; return sin(x); }
    };

    struct Cos {
        template<cg::Numeric T>
        T operator()(T x) const { using std::cos; return cos(x); }
    };

    struct Exp {
        template<cg::Numeric T>
        T operator()(T x) const { using std::exp; return exp(x); }
    };

    struct Log {
        template<cg::Numeric T>
        T operator()(T x) const { using std::log; return log(x); }
    };

    struct Pow {
        template<cg::Numeric T>
        T operator()(T base, T exponent) const { using std::pow; return pow(base, exponent); }
    };

    struct Sqrt {
        template<cg::Numeric T>
        T operator()(T x) const { using std::sqrt; return sqrt(x); }
    };



}