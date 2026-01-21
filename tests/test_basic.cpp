// #include <iostream>
// #include <cassert>
// #include <cmath>
// #include <vector>
// #include "cg/expression.hpp"
// #include "cg/eval/evaluator.hpp"
// #include "cg/eval/policies.hpp"
//
// #define TESTCASE(name) \
//     void name(); \
//     int main() { \
//         try { \
//             name(); \
//             std::cout << "[PASS] " << #name << "\n"; \
//         } catch (const std::exception& e) { \
//             std::cout << "[FAIL] " << #name << ": " << e.what() << "\n"; \
//             return 1; \
//         } \
//         return 0; \
//     } \
//     void name()
//
// bool precision(double a, double b, double epsilon = 1e-10) {
//     return std::abs(a - b) < epsilon;
// }
//
// TESTCASE(run_all) {
//     using T = double;
//
//     cg::Evaluator<T, cg::eval::NaiveEvaluator> evaluator;
//
//     // arithmetic & scalar mixing
//     {
//         cg::Graph<T> G;
//         auto x = cg::input(G, "x");
//         auto expr = x * 2.0 + 10.0;
//
//         cg::Context<T> ctx;
//         ctx["x"] = 5.0;
//         T result = evaluator.evaluate(G, expr.root(), ctx);
//         assert(precision(result, 20.0));
//     }
//
//     // cse
//     {
//         cg::Graph<T> G;
//         auto x = cg::input(G, "x");
//
//         auto sub = x + 1.0;
//         auto expr = sub * sub;
//
//         cg::Context<T> ctx;
//         ctx["x"] = 2.0;
//         T result = evaluator.evaluate(G, expr.root(), ctx);
//
//         assert(G.size() == 4);
//     }
// }