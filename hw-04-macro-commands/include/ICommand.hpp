#pragma once

#include <stdexcept>

/**
 * @file ICommand.hpp
 * @brief Интерфейс команды по паттерну Command
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Интерфейс команды по паттерну Command
  *
  * Обеспечивает инкапсуляцию операций в объекты, что позволяет
  * параметризовать клиентов с различными запросами, ставить запросы
  * в очередь и поддерживать отмену операций.
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