#pragma once

#include <stdexcept>

/**
 * @file IFuelable.hpp
 * @brief ��������� ��� ��������, ������������ �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ��������� ��� ��������, ������������ �������
  *
  * ������������ ���������� ��� ������ � �������� � ������� ��������.
  * ������� �������� Interface Segregation - �������� ������ ������,
  * ����������� ��� ���������� ��������.
  */
class IFuelable {
public:
    /**
     * @brief ����������� ����������
     */
    virtual ~IFuelable() = default;

    /**
     * @brief �������� ������� ������� �������
     * @return ���������� �������
     * @throws std::runtime_error ���� ������� ������� ���������� ���������
     */
    virtual int getFuelLevel() const = 0;

    /**
     * @brief ���������� ����� ������� �������
     * @param level ����� ������� �������
     * @throws std::runtime_error ���� ������� ������� ���������� ����������
     */
    virtual void setFuelLevel(int level) = 0;

    /**
     * @brief �������� ������ ������� �� ��������
     * @return ���������� �������, ����������� �� ���� ��������
     * @throws std::runtime_error ���� ������ ������� ���������� ���������
     */
    virtual int getFuelConsumption() const = 0;
};