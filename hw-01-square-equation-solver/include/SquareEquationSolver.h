#pragma once

#include <vector>

class SquareEquationSolver {
public:
	// ������ ���������� ��������� ax^2 + bx + c = 0
	std::vector<double> solve(double a, double b, double c) const;
};