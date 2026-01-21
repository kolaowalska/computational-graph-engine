#include <iostream>
#include "cg/expression.hpp"
#include "cg/eval/evaluator.hpp"
#include "cg/eval/policies.hpp"

int main() {
    using T = double;

    cg::Graph<T> G;

    // f(x, y) = sin(x) * (y + 2) + 3 * x * x
    auto x = cg::input(G, "x");
    auto y = cg::input(G, "y");

    auto expr = cg::sin(x) * (y + 2.0) + 3.0 * x * x;
    auto expression = cg::pi(G) + 1.0;

    cg::Context<T> ctx;

    std::cout << "x = "; std::cin >> ctx["x"];
    std::cout << "y = "; std::cin >> ctx["y"];

    //cg::Evaluator<T, cg::eval::NaiveEvaluator> eval;
    cg::Evaluator<T, cg::eval::NaiveEvaluator> naive;
    auto naive_result = naive.evaluate(G, expr.root(), ctx);

    cg::Evaluator<T, cg::eval::LazyEvaluator> lazy;
    auto lazy_result = lazy.evaluate(G, expr.root(), ctx);

    std::cout << "naive result = " << naive_result << "\n";
    std::cout << "lazy result = " << lazy_result << "\n";
    std::cout << "nodes = " << G.size() << "\n";
}