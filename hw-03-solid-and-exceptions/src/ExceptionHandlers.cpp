#include "ExceptionHandlers.hpp"
#include "LogCommand.hpp"
#include "RetryCommand.hpp"
#include "DoubleRetryCommand.hpp"
#include <iostream>

// ==================== LogExceptionHandler ====================

// ����������, ������� �������� ����������
LogExceptionHandler::LogExceptionHandler(CommandQueue* queue,
    const std::string& logFilePath)
    : queue_(queue), logFilePath_(logFilePath) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// ���������� ���������� - �������� LogCommand � �������
void LogExceptionHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // ������� ������� �����������
    auto logCommand = std::make_shared<LogCommand>(command, exception, logFilePath_);

    // ��������� � � �������
    queue_->push(logCommand);

    std::cout << "[LogExceptionHandler] Added LogCommand to queue" << std::endl;
}

// ==================== RetryExceptionHandler ====================

// ����������, ������� ��������� �������
RetryExceptionHandler::RetryExceptionHandler(CommandQueue* queue)
    : queue_(queue) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// ���������� ���������� - �������� RetryCommand � �������
void RetryExceptionHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // ������� ������� ����������
    auto retryCommand = std::make_shared<RetryCommand>(command);

    // ��������� � � �������
    queue_->push(retryCommand);

    std::cout << "[RetryExceptionHandler] Added RetryCommand to queue" << std::endl;
}

// ==================== RetryThenLogHandler ====================

// ����������, ������� ��������� �������, � ����� ��������
RetryThenLogHandler::RetryThenLogHandler(CommandQueue* queue,
    const std::string& logFilePath)
    : queue_(queue), logFilePath_(logFilePath) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// ���������� ���������� - ��������� ��� ����������
void RetryThenLogHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // ���������, �������� �� ������� ��� RetryCommand
    auto retryCmd = std::dynamic_pointer_cast<RetryCommand>(command);

    if (retryCmd) {
        // ��� ��� ��������� ������� - ��������
        // ��������� ������������ �������
        auto originalCommand = retryCmd->getInnerCommand();
        auto logCommand = std::make_shared<LogCommand>(originalCommand, exception, logFilePath_);
        queue_->push(logCommand);

        std::cout << "[RetryThenLogHandler] Second attempt failed - logging" << std::endl;
    }
    else {
        // ������ ������� - ���������
        auto retryCommand = std::make_shared<RetryCommand>(command);
        queue_->push(retryCommand);

        std::cout << "[RetryThenLogHandler] First attempt failed - retrying" << std::endl;
    }
}

// ==================== DoubleTryThenLogHandler ====================

// ����������, ������� ��������� ������� ������, � ����� ��������
DoubleTryThenLogHandler::DoubleTryThenLogHandler(CommandQueue* queue,
    const std::string& logFilePath)
    : queue_(queue), logFilePath_(logFilePath) {
    if (!queue) {
        throw std::invalid_argument("Queue cannot be null");
    }
}

// ���������� ���������� - ��������� ��� ����������
void DoubleTryThenLogHandler::handle(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    // ��������� ��� ������� ��� ����������� ���������� �������
    auto doubleRetryCmd = std::dynamic_pointer_cast<DoubleRetryCommand>(command);
    auto retryCmd = std::dynamic_pointer_cast<RetryCommand>(command);

    if (doubleRetryCmd) {
        // ������ ������� - ��������
        auto originalCommand = doubleRetryCmd->getInnerCommand();
        auto logCommand = std::make_shared<LogCommand>(originalCommand, exception, logFilePath_);
        queue_->push(logCommand);

        std::cout << "[DoubleTryThenLogHandler] Third attempt failed - logging" << std::endl;
    }
    else if (retryCmd) {
        // ������ ������� - ������� DoubleRetryCommand
        auto originalCommand = retryCmd->getInnerCommand();
        auto doubleRetry = std::make_shared<DoubleRetryCommand>(originalCommand);
        queue_->push(doubleRetry);

        std::cout << "[DoubleTryThenLogHandler] Second attempt failed - trying one more time" << std::endl;
    }
    else {
        // ������ ������� - ������� RetryCommand
        auto retryCommand = std::make_shared<RetryCommand>(command);
        queue_->push(retryCommand);

        std::cout << "[DoubleTryThenLogHandler] First attempt failed - retrying" << std::endl;
    }
}