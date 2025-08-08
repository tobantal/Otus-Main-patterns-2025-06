#pragma once

#include "ICommand.hpp"
#include "IMovable.hpp"
#include <memory>

/**
 * @file MoveCommand.hpp
 * @brief Команда для выполнения прямолинейного движения
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда для выполнения прямолинейного движения
  *
  * Реализует паттерн Command для операции движения.
  * Инкапсулирует логику перемещения объекта на один шаг.
  */
class MoveCommand : public ICommand {
private:
    std::shared_ptr<IMovable> m_movable; ///< Указатель на движущийся объект

public:
    /**
     * @brief Конструктор команды движения
     * @param movable Указатель на объект, который нужно переместить
     * @throws std::invalid_argument если movable равен nullptr
     */
    explicit MoveCommand(std::shared_ptr<IMovable> movable);

    /**
     * @brief Выполнить движение объекта
     *
     * Перемещает объект на расстояние, равное его скорости за один шаг времени.
     * Новая позиция = текущая позиция + скорость.
     *
     * @throws std::runtime_error если не удается выполнить движение
     */
    void execute() override;
};