#pragma once

#include <stdexcept>

/**
 * @file ICommand.hpp
 * @brief Интерфейс команды по паттерну Command
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Интерфейс команды по паттерну Command
 *
 * Обеспечивает инкапсуляцию операций в объекты
 */
class ICommand {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~ICommand() = default;

    /**
     * @brief Выполнить команду
     * @throws std::runtime_error если команду невозможно выполнить
     */
    virtual void execute() = 0;
};