#pragma once

#include "ICommand.hpp"
#include "IUObject.hpp"
#include <memory>

/**
 * @file StartMoveCommand.hpp
 * @brief Команда начала движения игрового объекта
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда начала движения
 * 
 * Устанавливает начальную скорость объекта.
 * Ожидает параметр "initialVelocity" в params.
 * 
 * После выполнения объект получает свойство "velocity"
 * с указанным значением начальной скорости.
 */
class StartMoveCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * 
     * @param object Объект для управления
     * @param params Параметры команды (должен содержать initialVelocity)
     * @throws std::runtime_error если параметр initialVelocity отсутствует
     */
    StartMoveCommand(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params);
    
    /**
     * @brief Выполняет команду начала движения
     * 
     * Устанавливает свойство "velocity" объекта.
     */
    void execute() override;

private:
    std::shared_ptr<IUObject> object_;  ///< Целевой объект
    int initialVelocity_;                ///< Начальная скорость
};