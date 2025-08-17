#pragma once

#include "ICommand.hpp"
#include "IFuelable.hpp"
#include "CommandException.hpp"
#include <memory>

/**
 * @file CheckFuelCommand.hpp
 * @brief Команда для проверки наличия топлива
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда для проверки наличия топлива
  *
  * Реализует паттерн Command для операции проверки топлива.
  * Выбрасывает исключение, если топлива недостаточно для операции.
  */
class CheckFuelCommand : public ICommand {
private:
    std::shared_ptr<IFuelable> m_fuelable; ///< Указатель на объект с топливом

public:
    /**
     * @brief Конструктор команды проверки топлива
     * @param fuelable Указатель на объект с топливом
     * @throws std::invalid_argument если fuelable равен nullptr
     */
    explicit CheckFuelCommand(std::shared_ptr<IFuelable> fuelable);

    /**
     * @brief Проверить наличие достаточного количества топлива
     *
     * Проверяет, что текущий уровень топлива не меньше расхода топлива.
     *
     * @throws CommandException если топлива недостаточно
     * @throws std::runtime_error если не удается выполнить проверку
     */
    void execute() override;
};