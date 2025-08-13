#include <iostream>
#include <memory>
#include "CommandQueue.hpp"
#include "ExceptionHandlers.hpp"
#include "TestCommands.hpp"
#include "LogCommand.hpp"
#include "RetryCommand.hpp"
#include "DoubleRetryCommand.hpp"

/**
 * @brief ������������ �������� �����������
 */
void demoSimpleLogging() {
    std::cout << "\n=== Demo: Simple Logging ===" << std::endl;

    CommandQueue queue;

    // ������� ���������� ��� �����������
    auto logHandler = std::make_shared<LogExceptionHandler>(&queue, "demo.log");
    queue.setDefaultHandler(logHandler);

    // ��������� �������
    queue.push(std::make_shared<SuccessCommand>("Command1"));
    queue.push(std::make_shared<FailCommand>("Command2", "Intentional failure"));
    queue.push(std::make_shared<SuccessCommand>("Command3"));

    // ������������ ��� �������
    queue.processAll();
}

/**
 * @brief ������������ �������� ����������
 */
void demoSimpleRetry() {
    std::cout << "\n=== Demo: Simple Retry ===" << std::endl;

    CommandQueue queue;

    // ������� ���������� ��� ����������
    auto retryHandler = std::make_shared<RetryExceptionHandler>(&queue);
    queue.setDefaultHandler(retryHandler);

    // �������, ������� ������� ����� 1 �������
    auto retryableCmd = std::make_shared<RetryableCommand>("RetryableTask", 1);
    queue.push(retryableCmd);

    // ������������
    int maxIterations = 10; // ������ �� ������������ �����
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }

    std::cout << "Command successful: " << retryableCmd->isSuccessful() << std::endl;
}

/**
 * @brief ������������ ��������� "���������, ����� ����������"
 */
void demoRetryThenLog() {
    std::cout << "\n=== Demo: Retry Then Log ===" << std::endl;

    CommandQueue queue;

    // ������� ���������� ��� ��������� "���������, ����� ����������"
    auto handler = std::make_shared<RetryThenLogHandler>(&queue, "demo.log");

    // ������������ ����������� ��� ���������� �����
    queue.registerHandler<FailCommand, std::runtime_error>(handler);
    queue.registerHandler<RetryCommand, std::runtime_error>(handler);

    // �������, ������� ������ ������ �������
    queue.push(std::make_shared<FailCommand>("AlwaysFailTask"));

    // ������������
    int maxIterations = 10;
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }
}

/**
 * @brief ������������ ��������� "��������� ������, ����� ����������"
 */
void demoDoubleTryThenLog() {
    std::cout << "\n=== Demo: Double Try Then Log ===" << std::endl;

    CommandQueue queue;

    // ������� ���������� ��� ��������� "��������� ������, ����� ����������"
    auto handler = std::make_shared<DoubleTryThenLogHandler>(&queue, "demo.log");

    // ������������ ����������� ��� ���� ����� ������
    queue.registerHandler<FailCommand, std::runtime_error>(handler);
    queue.registerHandler<RetryCommand, std::runtime_error>(handler);
    queue.registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // �������, ������� ������ ������ �������
    auto failCmd = std::make_shared<FailCommand>("PersistentFailure");
    queue.push(failCmd);

    // ������������
    int maxIterations = 10;
    while (!queue.isEmpty() && maxIterations-- > 0) {
        queue.processOne();
    }

    std::cout << "Total attempts: " << failCmd->getAttemptCount() << std::endl;
}

/**
 * @brief ������������ ��������� ���������� ����� ����������
 */
void demoSuccessfulRetry() {
    std::cout << "\n=== Demo: Successful Retry ===" << std::endl;

    CommandQueue queue;

    // ������� ���������� ��� ��������� "��������� ������, ����� ����������"
    auto handler = std::make_shared<DoubleTryThenLogHandler>(&queue, "demo.log");

    // ������������ �����������
    queue.registerHandler<RetryableCommand, std::runtime_error>(handler);
    queue.registerHandler<RetryCommand, std::runtime_error>(handler);
    queue.registerHandler<DoubleRetryCommand, std::runtime_error>(handler);

    // �������, ������� ������� ����� 2 ������
    auto retryableCmd = std::make_shared<RetryableCommand>("EventualSuccess", 2);
    queue.push(retryableCmd);

    // ������������
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
        // ������������� ��������� ��������� ��������� ����������
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