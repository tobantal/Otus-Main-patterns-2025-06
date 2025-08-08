#include <gtest/gtest.h>
#include "Vector2D.hpp"

/**
 * @brief Тестовый класс для Vector2D с целочисленными координатами
 */
class Vector2DTest : public ::testing::Test {
};

/**
 * @brief Тест конструктора по умолчанию
 */
TEST_F(Vector2DTest, DefaultConstructor) {
    Vector2D v;
    EXPECT_EQ(v.x, 0);
    EXPECT_EQ(v.y, 0);
}

/**
 * @brief Тест конструктора с параметрами
 */
TEST_F(Vector2DTest, ParameterizedConstructor) {
    Vector2D v(10, -5);
    EXPECT_EQ(v.x, 10);
    EXPECT_EQ(v.y, -5);
}

/**
 * @brief Тест конструктора с отрицательными значениями
 */
TEST_F(Vector2DTest, NegativeCoordinates) {
    Vector2D v(-15, -20);
    EXPECT_EQ(v.x, -15);
    EXPECT_EQ(v.y, -20);
}

/**
 * @brief Тест оператора сложения
 */
TEST_F(Vector2DTest, AdditionOperator) {
    Vector2D v1(10, 20);
    Vector2D v2(5, -15);
    Vector2D result = v1 + v2;

    EXPECT_EQ(result.x, 15);
    EXPECT_EQ(result.y, 5);
}

/**
 * @brief Тест оператора сложения с отрицательными числами
 */
TEST_F(Vector2DTest, AdditionWithNegatives) {
    Vector2D v1(12, 5);
    Vector2D v2(-7, 3);
    Vector2D result = v1 + v2;

    EXPECT_EQ(result.x, 5);  // 12 + (-7) = 5
    EXPECT_EQ(result.y, 8);  // 5 + 3 = 8
}

/**
 * @brief Тест оператора умножения на скаляр
 */
TEST_F(Vector2DTest, ScalarMultiplicationOperator) {
    Vector2D v(3, -4);
    Vector2D result = v * 5;

    EXPECT_EQ(result.x, 15);   // 3 * 5 = 15
    EXPECT_EQ(result.y, -20);  // -4 * 5 = -20
}

/**
 * @brief Тест умножения на ноль
 */
TEST_F(Vector2DTest, MultiplicationByZero) {
    Vector2D v(100, -50);
    Vector2D result = v * 0;

    EXPECT_EQ(result.x, 0);
    EXPECT_EQ(result.y, 0);
}

/**
 * @brief Тест умножения на отрицательное число
 */
TEST_F(Vector2DTest, MultiplicationByNegative) {
    Vector2D v(4, -6);
    Vector2D result = v * -3;

    EXPECT_EQ(result.x, -12);  // 4 * (-3) = -12
    EXPECT_EQ(result.y, 18);   // -6 * (-3) = 18
}

/**
 * @brief Тест оператора сравнения на равенство
 */
TEST_F(Vector2DTest, EqualityOperator) {
    Vector2D v1(10, 20);
    Vector2D v2(10, 20);
    Vector2D v3(11, 20);
    Vector2D v4(10, 21);

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 == v4);
}

/**
 * @brief Тест сравнения с отрицательными координатами
 */
TEST_F(Vector2DTest, EqualityWithNegatives) {
    Vector2D v1(-5, -10);
    Vector2D v2(-5, -10);
    Vector2D v3(5, -10);

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

/**
 * @brief Тест цепочки операций
 */
TEST_F(Vector2DTest, ChainedOperations) {
    Vector2D v1(1, 2);
    Vector2D v2(3, 4);
    Vector2D v3(5, 6);

    // Тест (v1 + v2) + v3
    Vector2D result1 = (v1 + v2) + v3;
    EXPECT_EQ(result1.x, 9);   // (1+3) + 5 = 9
    EXPECT_EQ(result1.y, 12);  // (2+4) + 6 = 12

    // Тест v1 + (v2 * 2)
    Vector2D result2 = v1 + (v2 * 2);
    EXPECT_EQ(result2.x, 7);   // 1 + (3*2) = 7
    EXPECT_EQ(result2.y, 10);  // 2 + (4*2) = 10
}

/**
 * @brief Тест граничных значений
 */
TEST_F(Vector2DTest, BoundaryValues) {
    // Тест с максимальными значениями int
    Vector2D large(1000000, -1000000);
    EXPECT_EQ(large.x, 1000000);
    EXPECT_EQ(large.y, -1000000);

    // Тест с нулевыми значениями
    Vector2D zero(0, 0);
    Vector2D nonZero(5, 3);
    Vector2D result = zero + nonZero;

    EXPECT_EQ(result.x, 5);
    EXPECT_EQ(result.y, 3);
}