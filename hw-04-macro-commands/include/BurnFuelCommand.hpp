#pragma once

#include "ICommand.hpp"
#include "IFuelable.hpp"
#include <memory>

/**
 * @file BurnFuelCommand.hpp
 * @brief Команда для сжигания топлива
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда для сжигания топлива
  *
  * Реализует паттерн Command для операции сжигания топлива.
  * Уменьшает количество топлива на величину расхода.
  */
class BurnFuelCommand : public ICommand {
private:
    std::shared_ptr<IFuelable> m_fuelable; ///< Указатель на объект с топливом

public:
    /**
     * @brief Конструктор команды сжигания топлива
     * @param fuelable Указатель на объект с топливом
     * @throws std::invalid_argument если fuelable равен nullptr
     */
    explicit BurnFuelCommand(std::shared_ptr<IFuelable> fuelable);

    /**
     * @brief Выполнить сжигание топлива
     *
     * Уменьшает текущий уровень топлива на величину расхода топлива.
     * Новый уровень = текущий уровень - расход.
     *
     * @throws std::runtime_error если не удается выполнить операцию
     */
    void execute() override;
};