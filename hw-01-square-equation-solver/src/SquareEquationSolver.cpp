#include "SquareEquationSolver.h"
#include <cmath>
#include <stdexcept>
#include "MathSettings.h"

std::vector<double> SquareEquationSolver::solve(double a, double b, double c) const
{
	// �������� ������������� �� isnan/inf
	if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c))
		throw std::invalid_argument("Coefficients must be finite numbers");

	// ���������, ����� �� ����������� a � ����������� ����
	if (std::abs(a) < EPS)
		throw std::invalid_argument("Coefficient 'a' must not be zero");

	// ��������� ������������
	double d = b * b - 4 * a * c;

	// ������������ ������������� (���� ����������� ����)
	if (d < -EPS) {
		// ��� ������
		return {};
	}
	// ������������ ������������� (�� �������� � ����������� ����)
	else if (d > EPS) {
		// ��� ��������� �����
		double sqrt_d = std::sqrt(d);
		return { (-b + sqrt_d) / (2 * a), (-b - sqrt_d) / (2 * a) };
	}
	// ���� ���������� ���� (� ����������� ����)
	else {
		// ������ �������� �����
		return { -b / (2 * a) };
	}
}