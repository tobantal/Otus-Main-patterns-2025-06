#pragma once

#include "Vector2D.hpp"
#include <stdexcept>

/**
 * @file IMovable.hpp
 * @brief ��������� ��� ��������, ��������� � �������������� ��������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ��������� ��� ��������, ��������� � �������������� ��������
  *
  * ������ ��������� ������������ ���������� ��� ���������, ������� �����
  * ������������ � ��������� ������������ � ���������� ���������.
  * ������� �������� Interface Segregation - �������� ������ ������,
  * ����������� ��� ��������.
  */
class IMovable {
public:
    /**
     * @brief ����������� ����������
     */
    virtual ~IMovable() = default;

    /**
     * @brief �������� ������� ������� �������
     * @return ������ ������� �������
     * @throws std::runtime_error ���� ������� ���������� ���������
     */
    virtual Vector2D getPosition() const = 0;

    /**
     * @brief ���������� ����� ������� �������
     * @param position ����� �������
     * @throws std::runtime_error ���� ������� ���������� ����������
     */
    virtual void setPosition(const Vector2D& position) = 0;

    /**
     * @brief �������� ������� �������� �������
     * @return ������ ��������
     * @throws std::runtime_error ���� �������� ���������� ���������
     */
    virtual Vector2D getVelocity() const = 0;
};