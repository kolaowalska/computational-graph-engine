#include <iostream>
#include <fstream>
#include <filesystem>
#include "cg/expression.hpp"
#include "cg/dual.hpp"
#include "cg/eval/evaluator.hpp"
#include "cg/eval/policies.hpp"
#include "cg/viz/visualizer.hpp"


void arithmetic() {
    using T = double;
    cg::Graph<T> G;

    auto x = cg::input(G, "x");
    auto y = cg::input(G, "y");

    std::cout << "evaluating expression f(x, y) = sin(x) * (y + 2) + 3 * x^2\n";

    auto expr = cg::sin(x) * (y + 2.0) + 3.0 * x * x;

    cg::Evaluator<T, cg::eval::NaiveEvaluator> naive;
    cg::Context<T> ctx;

    std::cout << "x = "; std::cin >> ctx["x"];
    std::cout << "y = "; std::cin >> ctx["y"];

    try {
        T result = naive.evaluate(G, expr.root(), ctx);
        std::cout << "result = " << result << "\n";
        cg::viz::visualize(G);
    } catch (const std::exception& e) {
        std::cerr << "error :( " << e.what() << "\n";
    }

    std::cin.clear();
    std::string ignore; std::getline(std::cin, ignore);

}

void ad() {
    using T = cg::Dual<double>;
    cg::Graph<T> H;

    auto x = cg::input(H, "x");
    auto y = cg::input(H, "y");

    std::cout << "evaluating derivative of expression f(x, y) = sin(x) + y^2\n";

    auto expr = cg::sin(x) + y * y;

    cg::Evaluator<T, cg::eval::NaiveEvaluator> naive;
    cg::Context<T> ctx;

    double input_x, input_y;

    std::cout << "x = "; std::cin >> input_x;
    std::cout << "y = "; std::cin >> input_y;

    ctx["x"] = {input_x, 1.0};
    ctx["y"] = {input_y, 1.0};

    try {
        T result = naive.evaluate(H, expr.root(), ctx);
        std::cout << "f(" << input_x << ", " << input_y << ") = " << result.value << "\n";
        std::cout << "f'(" << input_x << ", " << input_y << ") = " << result.d << "\n";
        cg::viz::visualize(H);
    } catch (const std::exception& e) {
        std::cerr << "error :( " << e.what() << "\n";
    }

    std::cin.clear();
    std::string ignore; std::getline(std::cin, ignore);

}


int main() {
    int choice = 0;
    while (true) {
        std::cout << "\nchoose an option:\n";
        std::cout << "1. standard math evaluation\n";
        std::cout << "2. automatic differentiation\n";
        std::cout << "3. exit\n";

        std::cin >> choice;

        if (choice == 1) {
            arithmetic();
            break;
        }
        if (choice == 2) {
            ad();
            break;
        }
        break;
    }
    return 0;
}