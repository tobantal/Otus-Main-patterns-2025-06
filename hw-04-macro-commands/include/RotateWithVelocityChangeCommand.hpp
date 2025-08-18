#pragma once

#include "MacroCommand.hpp"
#include "IRotatable.hpp"
#include "IVelocityChangeable.hpp"
#include "IFuelable.hpp"
#include "IGameObject.hpp"
#include <memory>

/**
 * @file RotateWithVelocityChangeCommand.hpp
 * @brief Команда поворота с изменением вектора скорости
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда поворота с изменением вектора скорости и расходом топлива
  *
  * Специализированная макрокоманда, которая выполняет:
  * 1. Проверку наличия топлива (опционально)
  * 2. Поворот объекта
  * 3. Сжигание топлива (опционально)
  * 4. Изменение вектора мгновенной скорости (если объект движется)
  *
  * Инкапсулирует логику поворота с учетом изменения скорости.
  */
class RotateWithVelocityChangeCommand : public MacroCommand {
public:
    /**
     * @brief Конструктор команды поворота с изменением скорости
     * @param rotatable Указатель на поворачивающийся объект
     * @param velocityChangeable Указатель на объект с изменяемой скоростью (может быть nullptr)
     * @param fuelable Указатель на объект с топливом (может быть nullptr)
     * @throws std::invalid_argument если rotatable равен nullptr
     */
    RotateWithVelocityChangeCommand(
        std::shared_ptr<IRotatable> rotatable,
        std::shared_ptr<IVelocityChangeable> velocityChangeable = nullptr,
        std::shared_ptr<IFuelable> fuelable = nullptr);

    /**
     * @brief Фабричный метод для создания команды из игрового объекта
     * @param gameObject Игровой объект
     * @param useFuel Использовать ли топливо при повороте
     * @return Указатель на созданную команду
     */
    static std::shared_ptr<RotateWithVelocityChangeCommand> create(
        std::shared_ptr<IGameObject> gameObject,
        bool useFuel = false);
};