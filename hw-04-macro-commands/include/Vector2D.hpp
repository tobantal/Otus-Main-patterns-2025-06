#pragma once

/**
 * @file Vector2D.hpp
 * @brief Двумерный вектор с целочисленными координатами
 * @author Space Battle Team
 * @version 2.0
 */

 /**
  * @brief Двумерный вектор с целочисленными координатами
  *
  * Использует целые числа для избежания ошибок округления
  * при многократных операциях движения.
  */
struct Vector2D {
    int x; ///< Координата X (целое число)
    int y; ///< Координата Y (целое число)

    /**
     * @brief Конструктор по умолчанию
     * @param x Координата X (по умолчанию 0)
     * @param y Координата Y (по умолчанию 0)
     */
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    /**
     * @brief Оператор сложения векторов
     * @param other Второй вектор
     * @return Результат сложения векторов
     */
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    /**
     * @brief Оператор умножения вектора на скаляр
     * @param scalar Скалярное значение
     * @return Результат умножения
     */
    Vector2D operator*(int scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    /**
     * @brief Оператор сравнения векторов на равенство
     * @param other Второй вектор
     * @return true если векторы равны, false иначе
     */
    bool operator==(const Vector2D& other) const {
        return (x == other.x) && (y == other.y);
    }
};