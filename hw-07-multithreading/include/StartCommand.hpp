#pragma once

#include "ICommand.hpp"
#include "CommandExecutor.hpp"
#include <memory>

/**
 * @file StartCommand.hpp
 * @brief Команда запуска потока выполнения
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Команда для запуска потока выполнения команд
 *
 * Реализует паттерн Command для операции старта потока.
 */
class StartCommand : public ICommand {
private:
    std::shared_ptr<CommandExecutor> executor_;

public:
    /**
     * @brief Конструктор
     * @param executor Исполнитель команд
     * @throws std::invalid_argument если executor равен nullptr
     */
    explicit StartCommand(std::shared_ptr<CommandExecutor> executor);

    /**
     * @brief Деструктор
     */
    ~StartCommand() override = default;

    /**
     * @brief Выполнить команду - запустить поток
     * @throws std::runtime_error если поток уже запущен
     */
    void execute() override;
};