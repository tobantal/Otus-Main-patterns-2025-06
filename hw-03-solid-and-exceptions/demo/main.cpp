#include <iostream>
#include <memory>
#include "CommandQueue.hpp"
#include "ExceptionHandlers.hpp"
#include "TestCommands.hpp"
#include "LogCommand.hpp"
#include "RetryCommand.hpp"
#include "DoubleRetryCommand.hpp"

/**
 * @brief Демонстрация простого логирования
 */
void demoSimpleLogging() {
    std::cout << "\n=== Demo: Simple Logging ===" << std::endl;

    CommandQueue queue;

    // Создаем обработчик для логирования
    auto logHandler = std::make_shared<LogExceptionHandler>(&queue, "demo.log");
    queue.setDefaultHandler(logHandler);

    // Добавляем команды
    queue.push(std::make_shared<SuccessCommand>("Command1"));
    queue.push(std::make_shared<FailCommand>("Command2", "Intentional failure"));
    queue.push(std::make_shared<SuccessCommand>("Command3"));

    // Обрабатываем все команды
    queue.processAll();
}

/**
 * @brief Демонстрация простого повторения
 */
void demoSimpleRetry() {
    std::cout << "\n=== Demo: Simple Retry ===" << std::endl;

    CommandQueue queue;

    // Создаем обработчик для повторения
    auto retryHandler = std::make_shared<RetryExceptionHandler>(&queue);
    queue.setDefaultHandler(retryHandler);

    // Команда, которая успешна после 1 неудачи
    auto retryableCmd = std::make_shared<RetryableCommand>("RetryableTask", 1);
    queue.push(retryableCmd);

    // Обрабатываем
    int maxIterations = 10; // Защита от бесконечного цикла
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }

    std::cout << "Command successful: " << retryableCmd->isSuccessful() << std::endl;
}

/**
 * @brief Демонстрация стратегии "повторить, затем логировать"
 */
void demoRetryThenLog() {
    std::cout << "\n=== Demo: Retry Then Log ===" << std::endl;

    CommandQueue queue;

    // Создаем обработчик для стратегии "повторить, затем логировать"
    auto handler = std::make_shared<RetryThenLogHandler>(&queue, "demo.log");

    // Регистрируем обработчики для конкретных типов
    queue.registerHandler<FailCommand, std::runtime_error>(handler);
    queue.registerHandler<RetryCommand, std::runtime_error>(handler);

    // Команда, которая всегда терпит неудачу
    queue.push(std::make_shared<FailCommand>("AlwaysFailTask"));

    // Обрабатываем
    int maxIterations = 10;
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }
}

/**
 * @brief Демонстрация стратегии "повторить дважды, затем логировать"
 */
void demoDoubleTryThenLog() {
    std::cout << "\n=== Demo: Double Try Then Log ===" << std::endl;

    CommandQueue queue;

    // Создаем обработчик для стратегии "повторить дважды, затем логировать"
    auto handler = std::make_shared<DoubleTryThenLogHandler>(&queue, "demo.log");

    // Регистрируем обработчики для всех типов команд
    queue.registerHandler<FailCommand, std::runtime_error>(handler);
    queue.registerHandler<RetryCommand, std::runtime_error>(handler);
    queue.registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // Команда, которая всегда терпит неудачу
    auto failCmd = std::make_shared<FailCommand>("PersistentFailure");
    queue.push(failCmd);

    // Обрабатываем
    int maxIterations = 10;
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }

    std::cout << "Total attempts: " << failCmd->getAttemptCount() << std::endl;
}

/**
 * @brief Демонстрация успешного выполнения после повторений
 */
void demoSuccessfulRetry() {
    std::cout << "\n=== Demo: Successful Retry ===" << std::endl;

    CommandQueue queue;

    // Создаем обработчик для стратегии "повторить дважды, затем логировать"
    auto handler = std::make_shared<DoubleTryThenLogHandler>(&queue, "demo.log");

    // Регистрируем обработчики
    queue.registerHandler<RetryableCommand, std::runtime_error>(handler);
    queue.registerHandler<RetryCommand, std::runtime_error>(handler);
    queue.registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // Команда, которая успешна после 2 неудач
    auto retryableCmd = std::make_shared<RetryableCommand>("EventualSuccess", 2);
    queue.push(retryableCmd);

    // Обрабатываем
    int maxIterations = 10;
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }

    std::cout << "Command successful: " << retryableCmd->isSuccessful()
        << ", attempts: " << retryableCmd->getAttemptCount() << std::endl;
}

int main() {
    std::cout << "=== Space Battle Exception Handling Demo ===" << std::endl;

    try {
        // Демонстрируем различные стратегии обработки исключений
        demoSimpleLogging();
        demoSimpleRetry();
        demoRetryThenLog();
        demoDoubleTryThenLog();
        demoSuccessfulRetry();

        std::cout << "\n=== All demos completed successfully ===" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}