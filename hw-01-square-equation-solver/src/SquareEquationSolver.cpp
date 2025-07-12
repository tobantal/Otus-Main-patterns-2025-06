#include "SquareEquationSolver.h"
#include <cmath>
#include <stdexcept>
#include "MathSettings.h"

std::vector<double> SquareEquationSolver::solve(double a, double b, double c) const
{
	// проверки коэффициентов на isnan/inf
	if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c))
		throw std::invalid_argument("Coefficients must be finite numbers");

	// проверяем, ледит ли коэффициент a в окрестности нуля
	if (std::abs(a) < EPS)
		throw std::invalid_argument("Coefficient 'a' must not be zero");

	// вычисляем дискриминант
	double d = b * b - 4 * a * c;

	// дискриминант отрицательный (ниже окрестности нуля)
	if (d < -EPS) {
		// нет корней
		return {};
	}
	// дискриминант положительный (не пападает в окрестности нуля)
	else if (d > EPS) {
		// два различных корня
		double sqrt_d = std::sqrt(d);
		return { (-b + sqrt_d) / (2 * a), (-b - sqrt_d) / (2 * a) };
	}
	// если дискриминт нуль (в окрестности нуля)
	else {
		// случай кратного корня
		return { -b / (2 * a) };
	}
}