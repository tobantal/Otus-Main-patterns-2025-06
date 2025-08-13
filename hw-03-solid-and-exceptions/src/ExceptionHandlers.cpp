#include "ExceptionHandlers.hpp"
#include "LogCommand.hpp"
#include "RetryCommand.hpp"
#include "DoubleRetryCommand.hpp"
#include <iostream>

// ==================== LogExceptionHandler ====================

// Обработчик, который логирует исключения
LogExceptionHandler::LogExceptionHandler(CommandQueue* queue,
    const std::string& logFilePath)
    : queue_(queue), logFilePath_(logFilePath) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// Обработать исключение - добавить LogCommand в очередь
void LogExceptionHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // Создаем команду логирования
    auto logCommand = std::make_shared<LogCommand>(command, exception, logFilePath_);

    // Добавляем её в очередь
    queue_->push(logCommand);

    std::cout << "[LogExceptionHandler] Added LogCommand to queue" << std::endl;
}

// ==================== RetryExceptionHandler ====================

// Обработчик, который повторяет команды
RetryExceptionHandler::RetryExceptionHandler(CommandQueue* queue)
    : queue_(queue) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// Обработать исключение - добавить RetryCommand в очередь
void RetryExceptionHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // Создаем команду повторения
    auto retryCommand = std::make_shared<RetryCommand>(command);

    // Добавляем её в очередь
    queue_->push(retryCommand);

    std::cout << "[RetryExceptionHandler] Added RetryCommand to queue" << std::endl;
}

// ==================== RetryThenLogHandler ====================

// Обработчик, который повторяет команды, а затем логирует
RetryThenLogHandler::RetryThenLogHandler(CommandQueue* queue,
    const std::string& logFilePath)
    : queue_(queue), logFilePath_(logFilePath) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// Обработать исключение - повторить или логировать
void RetryThenLogHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // Проверяем, является ли команда уже RetryCommand
    auto retryCmd = std::dynamic_pointer_cast<RetryCommand>(command);

    if (retryCmd) {
        // Это уже повторная попытка - логируем
        // Извлекаем оригинальную команду
        auto originalCommand = retryCmd->getInnerCommand();
        auto logCommand = std::make_shared<LogCommand>(originalCommand, exception, logFilePath_);
        queue_->push(logCommand);

        std::cout << "[RetryThenLogHandler] Second attempt failed - logging" << std::endl;
    }
    else {
        // Первая попытка - повторяем
        auto retryCommand = std::make_shared<RetryCommand>(command);
        queue_->push(retryCommand);

        std::cout << "[RetryThenLogHandler] First attempt failed - retrying" << std::endl;
    }
}

// ==================== DoubleTryThenLogHandler ====================

// Обработчик, который повторяет команды дважды, а затем логирует
DoubleTryThenLogHandler::DoubleTryThenLogHandler(CommandQueue* queue,
    const std::string& logFilePath)
    : queue_(queue), logFilePath_(logFilePath) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// Обработать исключение - повторить или логировать
void DoubleTryThenLogHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // Проверяем тип команды для определения количества попыток
    auto doubleRetryCmd = std::dynamic_pointer_cast<DoubleRetryCommand>(command);
    auto retryCmd = std::dynamic_pointer_cast<RetryCommand>(command);

    if (doubleRetryCmd) {
        // Третья попытка - логируем
        auto originalCommand = doubleRetryCmd->getInnerCommand();
        auto logCommand = std::make_shared<LogCommand>(originalCommand, exception, logFilePath_);
        queue_->push(logCommand);

        std::cout << "[DoubleTryThenLogHandler] Third attempt failed - logging" << std::endl;
    }
    else if (retryCmd) {
        // Вторая попытка - создаем DoubleRetryCommand
        auto originalCommand = retryCmd->getInnerCommand();
        auto doubleRetry = std::make_shared<DoubleRetryCommand>(originalCommand);
        queue_->push(doubleRetry);

        std::cout << "[DoubleTryThenLogHandler] Second attempt failed - trying one more time" << std::endl;
    }
    else {
        // Первая попытка - создаем RetryCommand
        auto retryCommand = std::make_shared<RetryCommand>(command);
        queue_->push(retryCommand);

        std::cout << "[DoubleTryThenLogHandler] First attempt failed - retrying" << std::endl;
    }
}