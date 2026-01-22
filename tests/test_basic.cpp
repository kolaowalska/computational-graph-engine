#include <iostream>
#include <cassert>
#include <cmath>
#include "cg/expression.hpp"
#include "cg/dual.hpp"
#include "cg/eval/evaluator.hpp"
#include "cg/eval/policies.hpp"

#define TESTCASE(name) void name()

bool approx(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) < eps;
}

TESTCASE(test_arithmetic) {
    using T = double;
    cg::Graph<T> G;
    auto x = cg::input(G, "x");
    auto y = cg::input(G, "y");
    auto expr = y / 4.0 - x * 2.0 + 10.0 ;
    cg::Evaluator<T, cg::eval::NaiveEvaluator> evaluator;
    cg::Context<T> ctx;
    ctx["x"] = 1.0;
    ctx["y"] = 4.0;
    assert(approx(evaluator.evaluate(G, expr.root(), ctx), 9.0));
}

TESTCASE(test_cse) {
    using T = double;
    cg::Graph<T> G;
    auto x = cg::input(G, "x");
    auto sub = x * 3.0;
    auto expr = sub * sub;
    assert(G.size() == 4);
}

TESTCASE(test_ad) {
    using T = cg::Dual<double>;
    cg::Graph<T> G;
    auto x = cg::input(G, "x");
    auto y = cg::input(G, "y");
    auto expr = cg::sin(y) + x * y;
    cg::Evaluator<T, cg::eval::NaiveEvaluator> evaluator;
    cg::Context<T> ctx;
    double xvalue = 1.0;
    double yvalue = 5.25;

    ctx["x"] = T(xvalue, 1.0);
    ctx["y"] = T(yvalue, 0.0);
    T x_result = evaluator.evaluate(G, expr.root(), ctx);

    ctx["x"] = T(xvalue, 0.0);
    ctx["y"] = T(yvalue, 1.0);
    T y_result = evaluator.evaluate(G, expr.root(), ctx);

    // std::cout << "f(" << xvalue << ", " << yvalue << ") = " << x_result.value << "\n";
    // std::cout << "df/dx = " << x_result.d << " (expected: 5.25)\n";
    // std::cout << "df/dy = " << y_result.d << " (expected: cos(5.25) + 1.0)\n";

    assert(approx(x_result.value, std::sin(yvalue) + xvalue * yvalue));
    assert(approx(x_result.d, yvalue));
    assert(approx(y_result.d, std::cos(yvalue) + xvalue));
}

int main() {
    test_arithmetic();
    test_cse();
    test_ad();
    std::cout << "all tests passed! <3" << std::endl;
    return 0;
}