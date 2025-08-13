#pragma once

#include <memory>
#include <exception>

// Forward declaration of ICommand to avoid circular dependency
class ICommand;

/**
 * @file IExceptionHandler.hpp
 * @brief Интерфейс обработчика исключений
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Интерфейс для обработки исключений, возникающих при выполнении команд
  *
  * Позволяет реализовать различные стратегии обработки исключений
  * в соответствии с принципом Open-Closed (SOLID)
  */
class IExceptionHandler {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IExceptionHandler() = default;

    /**
     * @brief Обработать исключение
     * @param command Команда, которая выбросила исключение
     * @param exception Перехваченное исключение
     */
    virtual void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) = 0;
};