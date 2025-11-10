#pragma once

#include "StateChangeCommand.hpp"

/**
 * @file RunCommand.hpp
 * @brief Команда возврата в обычное состояние
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда для возврата в нормальное состояние
 * 
 * При выполнении переводит исполнитель обратно в состояние NormalState,
 * где команды снова начинают выполняться обычным образом.
 * Используется для выхода из состояния MoveTo.
 */
class RunCommand : public StateChangeCommand {
public:
    /**
     * @brief Конструктор
     * 
     * @param executor Исполнитель команд
     * @throws std::invalid_argument если executor равен nullptr
     */
    explicit RunCommand(std::shared_ptr<CommandExecutor> executor);
    
    /**
     * @brief Деструктор
     */
    ~RunCommand() override = default;
};
