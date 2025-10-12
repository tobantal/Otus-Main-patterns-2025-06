#pragma once

/**
 * @file Vector2D.hpp
 * @brief Двумерный вектор
 * @author Anton Tobolkin
 * @version 3.0
 */
struct Vector2D {
    int x; ///< первая координата
    int y; ///< вторая координата

    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator*(int scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    bool operator==(const Vector2D& other) const {
        return (x == other.x) && (y == other.y);
    }
};