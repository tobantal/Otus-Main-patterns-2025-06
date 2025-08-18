#pragma once

#include "MacroCommand.hpp"
#include "IRotatable.hpp"
#include "IVelocityChangeable.hpp"
#include "IFuelable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file RotateWithVelocityChangeCommand.hpp
 * @brief ������� �������� � ���������� ������� ��������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� �������� � ���������� ������� �������� � �������� �������
  *
  * ������������������ ������������, ������� ���������:
  * 1. �������� ������� ������� (�����������)
  * 2. ������� �������
  * 3. �������� ������� (�����������)
  * 4. ��������� ������� ���������� �������� (���� ������ ��������)
  *
  * ������������� ������ �������� � ������ ��������� ��������.
  */
class RotateWithVelocityChangeCommand : public MacroCommand {
public:
    /**
     * @brief ����������� ������� �������� � ���������� ��������
     * @param rotatable ��������� �� ���������������� ������
     * @param velocityChangeable ��������� �� ������ � ���������� ��������� (����� ���� nullptr)
     * @param fuelable ��������� �� ������ � �������� (����� ���� nullptr)
     * @throws std::invalid_argument ���� rotatable ����� nullptr
     */
    RotateWithVelocityChangeCommand(
        std::shared_ptr<IRotatable> rotatable,
        std::shared_ptr<IVelocityChangeable> velocityChangeable = nullptr,
        std::shared_ptr<IFuelable> fuelable = nullptr);

    /**
     * @brief ��������� ����� ��� �������� ������� �� �������� �������
     * @param gameObject ������� ������
     * @param useFuel ������������ �� ������� ��� ��������
     * @return ��������� �� ��������� �������
     */
    static std::shared_ptr<RotateWithVelocityChangeCommand> create(
        std::shared_ptr<IGameObject> gameObject,
        bool useFuel = false);
};