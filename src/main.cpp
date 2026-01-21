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

    // TODO: fix this
    auto expr = cg::sin(x) * (y + 2.0) + 3.0 * x * x;

    cg::Context<T> ctx;

    std::cout << "x = "; std::cin >> ctx["x"];
    std::cout << "y = "; std::cin >> ctx["y"];

    cg::Evaluator<T, cg::eval::NaiveEvaluator> eval;
    auto result = eval.evaluate(G, expr.root(), ctx);

    std::cout << "result = " << result << "\n";
    std::cout << "nodes  = " << G.size() << "\n";
}