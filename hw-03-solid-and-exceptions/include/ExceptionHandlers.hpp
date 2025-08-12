#pragma once

#include "IExceptionHandler.hpp"
#include "CommandQueue.hpp"
#include <memory>
#include <string>

/**
 * @file ExceptionHandlers.hpp
 * @brief Различные стратегии обработки исключений
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Обработчик, который логирует исключение
  *
  * Создает LogCommand и добавляет её в очередь команд
  */
class LogExceptionHandler : public IExceptionHandler {
public:
    /**
     * @brief Конструктор
     * @param queue Очередь команд для добавления LogCommand
     * @param logFilePath Путь к файлу лога
     */
    explicit LogExceptionHandler(CommandQueue* queue,
        const std::string& logFilePath = "error.log");

    /**
     * @brief Деструктор
     */
    ~LogExceptionHandler() override = default;

    /**
     * @brief Обработать исключение - добавить LogCommand в очередь
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
    std::string logFilePath_;
};

/**
 * @brief Обработчик, который повторяет команду
 *
 * Создает RetryCommand и добавляет её в очередь команд
 */
class RetryExceptionHandler : public IExceptionHandler {
public:
    /**
     * @brief Конструктор
     * @param queue Очередь команд для добавления RetryCommand
     */
    explicit RetryExceptionHandler(CommandQueue* queue);

    /**
     * @brief Деструктор
     */
    ~RetryExceptionHandler() override = default;

    /**
     * @brief Обработать исключение - добавить RetryCommand в очередь
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
};

/**
 * @brief Обработчик для стратегии "повторить, затем логировать"
 *
 * При первом исключении повторяет команду,
 * при повторном - логирует
 */
class RetryThenLogHandler : public IExceptionHandler {
public:
    /**
     * @brief Конструктор
     * @param queue Очередь команд
     * @param logFilePath Путь к файлу лога
     */
    explicit RetryThenLogHandler(CommandQueue* queue,
        const std::string& logFilePath = "error.log");

    /**
     * @brief Деструктор
     */
    ~RetryThenLogHandler() override = default;

    /**
     * @brief Обработать исключение
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
    std::string logFilePath_;
};

/**
 * @brief Обработчик для стратегии "повторить дважды, затем логировать"
 *
 * При первом исключении повторяет команду,
 * при втором - повторяет еще раз с маркером DoubleRetryCommand,
 * при третьем - логирует
 */
class DoubleTryThenLogHandler : public IExceptionHandler {
public:
    /**
     * @brief Конструктор
     * @param queue Очередь команд
     * @param logFilePath Путь к файлу лога
     */
    explicit DoubleTryThenLogHandler(CommandQueue* queue,
        const std::string& logFilePath = "error.log");

    /**
     * @brief Деструктор
     */
    ~DoubleTryThenLogHandler() override = default;

    /**
     * @brief Обработать исключение
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
    std::string logFilePath_;
};