#pragma once

#include "Vector2D.hpp"
#include <stdexcept>

/**
 * @file IVelocityChangeable.hpp
 * @brief ��������� ��� �������� � ���������� ���������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ��������� ��� �������� � ���������� ���������
  *
  * ������������ ���������� ��� ��������, � ������� ����� ��������
  * ����������� ������� �������� ��� ���������� ������.
  */
class IVelocityChangeable {
public:
    virtual ~IVelocityChangeable() = default;

    /**
     * @brief �������� ������ ��������
     * @return ������ �������� (������)
     * @throws std::runtime_error ���� �������� ���������� ���������
     */
    virtual int getVelocity() const = 0;

    /**
     * @brief �������� ������� �����������
     * @return ����������� � ���������� ��������
     * @throws std::runtime_error ���� ����������� ���������� ���������
     */
    virtual int getDirection() const = 0;

    /**
     * @brief �������� ����� ���������� ��������� �����������
     * @return ���������� ���������� �����������
     * @throws std::runtime_error ���� �������� ���������� ���������
     */
    virtual int getDirectionsNumber() const = 0;
};