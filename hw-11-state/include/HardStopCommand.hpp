#pragma once

#include "StateChangeCommand.hpp"

/**
 * @file HardStopCommand.hpp
 * @brief Команда остановки
 * @author Anton Tobolkin
 * @version 1.0
 */
/**
 * @brief Команда для жесткой остановки исполнителя
 */
class HardStopCommand : public StateChangeCommand {
public:
    /**
     * @brief Конструктор
     * 
     * @param executor Исполнитель команд
     * @throws std::invalid_argument если executor равен nullptr
     */
    explicit HardStopCommand(std::shared_ptr<CommandExecutor> executor);
    
    /**
     * @brief Деструктор
     */
    ~HardStopCommand() override = default;
};
