#pragma once

#include "ICommand.hpp"
#include "IRotatable.hpp"
#include <memory>

/**
 * @file RotateCommand.hpp
 * @brief Команда для выполнения поворота объекта
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда для выполнения поворота объекта
  *
  * Реализует паттерн Command для операции поворота.
  * Инкапсулирует логику поворота объекта на один шаг.
  */
class RotateCommand : public ICommand {
private:
    std::shared_ptr<IRotatable> m_rotatable; ///< Указатель на поворачивающийся объект

public:
    /**
     * @brief Конструктор команды поворота
     * @param rotatable Указатель на объект, который нужно повернуть
     * @throws std::invalid_argument если rotatable равен nullptr
     */
    explicit RotateCommand(std::shared_ptr<IRotatable> rotatable);

    /**
     * @brief Выполнить поворот объекта
     *
     * Поворачивает объект на угол, равный его угловой скорости за один шаг времени.
     * Новое направление = (текущее направление + угловая скорость) % количество направлений.
     *
     * @throws std::runtime_error если не удается выполнить поворот
     */
    void execute() override;
};