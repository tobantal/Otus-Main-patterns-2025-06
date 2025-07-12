#include <gtest/gtest.h>
#include "SquareEquationSolver.h"
#include "MathSettings.h"
#include <limits>

// Тест: уравнение x² + 1 = 0 (нет вещественных корней)
TEST(SquareEquationSolverTest, NoRealRoots) {
	SquareEquationSolver solver;
	auto result = solver.solve(1.0, 0.0, 1.0);
	EXPECT_TRUE(result.empty());
}

// Тест: x² - 1 = 0 → корни: x = 1 и -1
TEST(SquareEquationSolverTest, TwoDistinctRoots) {
	SquareEquationSolver solver;
	auto result = solver.solve(1.0, 0.0, -1.0); // x^2 - 1 = 0 → x = ±1
	ASSERT_EQ(result.size(), 2);
	EXPECT_NEAR(result[0], 1.0, EPS);
	EXPECT_NEAR(result[1], -1.0, EPS);
}

// Тест: x² + 2x + 1 = 0 → x = -1, кратность 2
TEST(SquareEquationSolverTest, DoubleRoot) {
	SquareEquationSolver solver;
	auto result = solver.solve(1.0, 2.0, 1.0); // дискриминант = 0
	ASSERT_EQ(result.size(), 1);
	EXPECT_NEAR(result[0], -1.0, EPS);
}

// Тест: a == 0 → выбрасываем исключение
TEST(SquareEquationSolverTest, CoefficientAZeroThrows) {
	SquareEquationSolver solver;
	EXPECT_THROW(solver.solve(0.0, 1.0, 1.0), std::invalid_argument);
}

//  Выбираем такие коэффициенты, что дискриминант очень мал, но не ноль
// D = b² - 4ac = 0.0000000001 → меньше EPS → считать как 1 корень
TEST(SquareEquationSolverTest, DoubleRootWithSmallDiscriminant) {
	SquareEquationSolver solver;
	double a = 1.0;
	double b = 2.0000000001;
	double c = 1.0;

	auto result = solver.solve(a, b, c);

	ASSERT_EQ(result.size(), 1);
	EXPECT_NEAR(result[0], -1.0, EPS);
}

// Тест: если любой коэффициент — nan или inf, выбрасывается исключение
TEST(SquareEquationSolverTest, NaNOrInfCoefficients) {
	SquareEquationSolver solver;

	constexpr double nan = std::numeric_limits<double>::quiet_NaN();
	constexpr double inf = std::numeric_limits<double>::infinity();

	EXPECT_THROW(solver.solve(nan, 1.0, 1.0), std::invalid_argument);
	EXPECT_THROW(solver.solve(1.0, inf, 1.0), std::invalid_argument);
	EXPECT_THROW(solver.solve(1.0, 1.0, -inf), std::invalid_argument);
}
