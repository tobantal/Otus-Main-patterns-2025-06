#pragma once

#include "ICommand.hpp"
#include "CommandExecutor.hpp"
#include <memory>

/**
 * @file HardStopCommand.hpp
 * @brief Команда жесткой остановки потока
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда для жесткой остановки потока выполнения
 *
 * Останавливает поток немедленно, не дожидаясь завершения команд в очереди.
 */
class HardStopCommand : public ICommand {
private:
    std::shared_ptr<CommandExecutor> executor_;

public:
    /**
     * @brief Конструктор
     * @param executor Исполнитель команд
     * @throws std::invalid_argument если executor равен nullptr
     */
    explicit HardStopCommand(std::shared_ptr<CommandExecutor> executor);

    /**
     * @brief Деструктор
     */
    ~HardStopCommand() override = default;

    /**
     * @brief Выполнить команду - жесткая остановка потока
     */
    void execute() override;
};