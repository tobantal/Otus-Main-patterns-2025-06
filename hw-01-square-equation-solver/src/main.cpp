#include <iostream>
#include "SquareEquationSolver.h"

int main() {
    SquareEquationSolver solver;
    double a, b, c;

    std::cout << "Solving the quadratic equation: ax^2 + bx + c = 0\n";
    std::cout << "Enter coefficients:\n";
    std::cout << "a = ";
    std::cin >> a;
    std::cout << "b = ";
    std::cin >> b;
    std::cout << "c = ";
    std::cin >> c;

    try {
        auto roots = solver.solve(a, b, c);

        if (roots.empty()) {
            std::cout << "The equation has no real roots.\n";
        }
        else if (roots.size() == 1) {
            std::cout << "The equation has one real root (double root): x = " << roots[0] << "\n";
        }
        else {
            std::cout << "The equation has two real roots: x1 = " << roots[0] << ", x2 = " << roots[1] << "\n";
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    return 0;
}