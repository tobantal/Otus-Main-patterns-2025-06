#pragma once

#include <stdexcept>

/**
 * @file IRotatable.hpp
 * @brief ��������� ��� ��������, ��������� � ��������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ��������� ��� ��������, ��������� � ��������
  *
  * ������������ ���������� ��� ���������, ������� ����� ��������������
  * ����������� ������. ���������� ������������� ������������� �����������
  * ��� ��������� ������ ���������� ��� ������������ ���������.
  */
class IRotatable {
public:
    /**
     * @brief ����������� ����������
     */
    virtual ~IRotatable() = default;

    /**
     * @brief �������� ������� ����������� �������
     * @return ����������� ��� ����� ����� (0 �� directionsNumber-1)
     * @throws std::runtime_error ���� ����������� ���������� ���������
     */
    virtual int getDirection() const = 0;

    /**
     * @brief ���������� ����� ����������� �������
     * @param direction ����� �����������
     * @throws std::runtime_error ���� ����������� ���������� ����������
     */
    virtual void setDirection(int direction) = 0;

    /**
     * @brief �������� ������� �������� �������
     * @return ������� �������� � ���������� �������� �������� �� ���
     * @throws std::runtime_error ���� ������� �������� ���������� ���������
     */
    virtual int getAngularVelocity() const = 0;

    /**
     * @brief �������� ����� ���������� ��������� �����������
     * @return ���������� ���������� ����������� (��������, 360 ��� ��������)
     */
    virtual int getDirectionsNumber() const = 0;
};