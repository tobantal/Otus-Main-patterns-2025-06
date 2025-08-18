#pragma once

#include "MacroCommand.hpp"
#include "IMovable.hpp"
#include "IFuelable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file MoveWithFuelCommand.hpp
 * @brief Команда движения с расходом топлива
 * @author Space Battle Team
 * @version 1.0
  *
  * Специализированная макрокоманда, которая выполняет:
  * 1. Проверку наличия топлива
  * 2. Движение объекта
  * 3. Сжигание топлива
  *
  * Инкапсулирует логику движения с учетом топлива.
  */
class MoveWithFuelCommand : public MacroCommand {
public:
    /**
     * @brief Конструктор команды движения с топливом
     * @param movable Указатель на движущийся объект
     * @param fuelable Указатель на объект с топливом
     * @throws std::invalid_argument если любой из указателей равен nullptr
     */
    MoveWithFuelCommand(std::shared_ptr<IMovable> movable,
        std::shared_ptr<IFuelable> fuelable);

    /**
     * @brief Фабричный метод для создания команды из игрового объекта
     * @param gameObject Игровой объект
     * @return Указатель на созданную команду
     */
    static std::shared_ptr<MoveWithFuelCommand> create(
        std::shared_ptr<IGameObject> gameObject);
};