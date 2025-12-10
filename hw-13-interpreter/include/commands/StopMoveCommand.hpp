#pragma once

#include "ICommand.hpp"
#include "IUObject.hpp"
#include <memory>

/**
 * @file StopMoveCommand.hpp
 * @brief Команда остановки движения игрового объекта
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда остановки движения
 * 
 * Устанавливает скорость объекта в 0.
 * Не требует дополнительных параметров.
 */
class StopMoveCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * 
     * @param object Объект для управления
     * @param params Параметры команды (не используются)
     */
    StopMoveCommand(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params);
    
    /**
     * @brief Выполняет команду остановки
     * 
     * Устанавливает свойство "velocity" объекта в 0.
     */
    void execute() override;

private:
    std::shared_ptr<IUObject> object_;  ///< Целевой объект
};