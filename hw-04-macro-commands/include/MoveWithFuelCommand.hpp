#pragma once

#include "MacroCommand.hpp"
#include "IMovable.hpp"
#include "IFuelable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file MoveWithFuelCommand.hpp
 * @brief ������� �������� � �������� �������
 * @author Space Battle Team
 * @version 1.0
  *
  * ������������������ ������������, ������� ���������:
  * 1. �������� ������� �������
  * 2. �������� �������
  * 3. �������� �������
  *
  * ������������� ������ �������� � ������ �������.
  */
class MoveWithFuelCommand : public MacroCommand {
public:
    /**
     * @brief ����������� ������� �������� � ��������
     * @param movable ��������� �� ���������� ������
     * @param fuelable ��������� �� ������ � ��������
     * @throws std::invalid_argument ���� ����� �� ���������� ����� nullptr
     */
    MoveWithFuelCommand(std::shared_ptr<IMovable> movable,
        std::shared_ptr<IFuelable> fuelable);

    /**
     * @brief ��������� ����� ��� �������� ������� �� �������� �������
     * @param gameObject ������� ������
     * @return ��������� �� ��������� �������
     */
    static std::shared_ptr<MoveWithFuelCommand> create(
        std::shared_ptr<IGameObject> gameObject);
};