#pragma once

#include <stdexcept>

/**
 * @file IFuelable.hpp
 * @brief Интерфейс для объектов, использующих топливо
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Интерфейс для объектов, использующих топливо
  *
  * Обеспечивает абстракцию для работы с топливом в игровых объектах.
  * Следует принципу Interface Segregation - содержит только методы,
  * необходимые для управления топливом.
  */
class IFuelable {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IFuelable() = default;

    /**
     * @brief Получить текущий уровень топлива
     * @return Количество топлива
     * @throws std::runtime_error если уровень топлива невозможно прочитать
     */
    virtual int getFuelLevel() const = 0;

    /**
     * @brief Установить новый уровень топлива
     * @param level Новый уровень топлива
     * @throws std::runtime_error если уровень топлива невозможно установить
     */
    virtual void setFuelLevel(int level) = 0;

    /**
     * @brief Получить расход топлива за операцию
     * @return Количество топлива, расходуемое за одну операцию
     * @throws std::runtime_error если расход топлива невозможно прочитать
     */
    virtual int getFuelConsumption() const = 0;
};