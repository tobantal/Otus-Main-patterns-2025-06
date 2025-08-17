#pragma once

#include "ICommand.hpp"
#include "IVelocityChangeable.hpp"
#include <memory>

/**
 * @file ChangeVelocityCommand.hpp
 * @brief Команда для изменения вектора скорости при повороте
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда для изменения вектора скорости при повороте
  *
  * Модифицирует вектор мгновенной скорости в случае поворота объекта.
  * Сохраняет модуль скорости, но изменяет направление в соответствии
  * с новым направлением объекта.
  * Учитывает, что не каждый разворачивающийся объект движется.
  */
class ChangeVelocityCommand : public ICommand {
private:
    std::shared_ptr<IVelocityChangeable> m_velocityChangeable;

public:
    /**
     * @brief Конструктор команды изменения скорости
     * @param velocityChangeable Указатель на объект с изменяемой скоростью
     * @throws std::invalid_argument если velocityChangeable равен nullptr
     */
    explicit ChangeVelocityCommand(std::shared_ptr<IVelocityChangeable> velocityChangeable);

    /**
     * @brief Выполнить изменение вектора скорости
     *
     * Пересчитывает вектор скорости на основе текущего направления объекта.
     * Если объект не движется (скорость = 0), команда ничего не делает.
     *
     * @throws std::runtime_error если не удается выполнить операцию
     */
    void execute() override;
};