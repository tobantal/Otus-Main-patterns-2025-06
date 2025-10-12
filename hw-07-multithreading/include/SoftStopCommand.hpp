#pragma once

#include "ICommand.hpp"
#include "CommandExecutor.hpp"
#include <memory>

/**
 * @file SoftStopCommand.hpp
 * @brief Команда мягкой остановки потока
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда для мягкой остановки потока выполнения
 *
 * Останавливает поток только после завершения всех команд в очереди.
 */
class SoftStopCommand : public ICommand {
private:
    std::shared_ptr<CommandExecutor> executor_;

public:
    /**
     * @brief Конструктор
     * @param executor Исполнитель команд
     * @throws std::invalid_argument если executor равен nullptr
     */
    explicit SoftStopCommand(std::shared_ptr<CommandExecutor> executor);

    /**
     * @brief Деструктор
     */
    ~SoftStopCommand() override = default;

    /**
     * @brief Выполнить команду - мягкая остановка потока
     */
    void execute() override;
};