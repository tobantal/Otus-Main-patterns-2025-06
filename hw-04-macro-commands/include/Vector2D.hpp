#pragma once

/**
 * @file Vector2D.hpp
 * @brief ��������� ������ � �������������� ������������
 * @author Space Battle Team
 * @version 2.0
 */

 /**
  * @brief ��������� ������ � �������������� ������������
  *
  * ���������� ����� ����� ��� ��������� ������ ����������
  * ��� ������������ ��������� ��������.
  */
struct Vector2D {
    int x; ///< ���������� X (����� �����)
    int y; ///< ���������� Y (����� �����)

    /**
     * @brief ����������� �� ���������
     * @param x ���������� X (�� ��������� 0)
     * @param y ���������� Y (�� ��������� 0)
     */
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    /**
     * @brief �������� �������� ��������
     * @param other ������ ������
     * @return ��������� �������� ��������
     */
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    /**
     * @brief �������� ��������� ������� �� ������
     * @param scalar ��������� ��������
     * @return ��������� ���������
     */
    Vector2D operator*(int scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    /**
     * @brief �������� ��������� �������� �� ���������
     * @param other ������ ������
     * @return true ���� ������� �����, false �����
     */
    bool operator==(const Vector2D& other) const {
        return (x == other.x) && (y == other.y);
    }
};