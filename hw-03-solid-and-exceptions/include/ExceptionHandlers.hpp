#pragma once

#include "IExceptionHandler.hpp"
#include "CommandQueue.hpp"
#include <memory>
#include <string>

/**
 * @file ExceptionHandlers.hpp
 * @brief ��������� ��������� ��������� ����������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ����������, ������� �������� ����������
  *
  * ������� LogCommand � ��������� � � ������� ������
  */
class LogExceptionHandler : public IExceptionHandler {
public:
    /**
     * @brief �����������
     * @param queue ������� ������ ��� ���������� LogCommand
     * @param logFilePath ���� � ����� ����
     */
    explicit LogExceptionHandler(CommandQueue* queue,
        const std::string& logFilePath = "error.log");

    /**
     * @brief ����������
     */
    ~LogExceptionHandler() override = default;

    /**
     * @brief ���������� ���������� - �������� LogCommand � �������
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
    std::string logFilePath_;
};

/**
 * @brief ����������, ������� ��������� �������
 *
 * ������� RetryCommand � ��������� � � ������� ������
 */
class RetryExceptionHandler : public IExceptionHandler {
public:
    /**
     * @brief �����������
     * @param queue ������� ������ ��� ���������� RetryCommand
     */
    explicit RetryExceptionHandler(CommandQueue* queue);

    /**
     * @brief ����������
     */
    ~RetryExceptionHandler() override = default;

    /**
     * @brief ���������� ���������� - �������� RetryCommand � �������
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
};

/**
 * @brief ���������� ��� ��������� "���������, ����� ����������"
 *
 * ��� ������ ���������� ��������� �������,
 * ��� ��������� - ��������
 */
class RetryThenLogHandler : public IExceptionHandler {
public:
    /**
     * @brief �����������
     * @param queue ������� ������
     * @param logFilePath ���� � ����� ����
     */
    explicit RetryThenLogHandler(CommandQueue* queue,
        const std::string& logFilePath = "error.log");

    /**
     * @brief ����������
     */
    ~RetryThenLogHandler() override = default;

    /**
     * @brief ���������� ����������
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
    std::string logFilePath_;
};

/**
 * @brief ���������� ��� ��������� "��������� ������, ����� ����������"
 *
 * ��� ������ ���������� ��������� �������,
 * ��� ������ - ��������� ��� ��� � �������� DoubleRetryCommand,
 * ��� ������� - ��������
 */
class DoubleTryThenLogHandler : public IExceptionHandler {
public:
    /**
     * @brief �����������
     * @param queue ������� ������
     * @param logFilePath ���� � ����� ����
     */
    explicit DoubleTryThenLogHandler(CommandQueue* queue,
        const std::string& logFilePath = "error.log");

    /**
     * @brief ����������
     */
    ~DoubleTryThenLogHandler() override = default;

    /**
     * @brief ���������� ����������
     */
    void handle(std::shared_ptr<ICommand> command,
        const std::exception& exception) override;

private:
    CommandQueue* queue_;
    std::string logFilePath_;
};