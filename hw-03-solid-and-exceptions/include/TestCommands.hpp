#pragma once

#include "ICommand.hpp"
#include <string>
#include <atomic>

/**
 * @file TestCommands.hpp
 * @brief �������� ������� ��� ������������ ������ �������
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� �������, ������� ������ ����������� �������
  */
class SuccessCommand : public ICommand {
public:
    /**
     * @brief �����������
     * @param name ��� ������� ��� �������������
     */
    explicit SuccessCommand(const std::string& name);

    /**
     * @brief ����������
     */
    ~SuccessCommand() override = default;

    /**
     * @brief ��������� �������
     */
    void execute() override;

    /**
     * @brief �������� ��� �������
     */
    const std::string& getName() const;

    /**
     * @brief �������� ���������� ����������
     */
    int getExecutionCount() const;

private:
    std::string name_;
    mutable std::atomic<int> executionCount_;
};

/**
 * @brief �������, ������� ������ ����������� ����������
 */
class FailCommand : public ICommand {
public:
    /**
     * @brief �����������
     * @param name ��� �������
     * @param errorMessage ��������� �� ������
     */
    explicit FailCommand(const std::string& name,
        const std::string& errorMessage = "Command failed");

    /**
     * @brief ����������
     */
    ~FailCommand() override = default;

    /**
     * @brief ��������� ������� - ������ ����������� ����������
     */
    void execute() override;

    /**
     * @brief �������� ��� �������
     */
    const std::string& getName() const;

    /**
     * @brief �������� ���������� ������� ����������
     */
    int getAttemptCount() const;

private:
    std::string name_;
    std::string errorMessage_;
    mutable std::atomic<int> attemptCount_;
};

/**
 * @brief �������, ������� ����������� ���������� ������ N ���
 */
class RetryableCommand : public ICommand {
public:
    /**
     * @brief �����������
     * @param name ��� �������
     * @param failuresBeforeSuccess ���������� ������ ����� �������
     */
    explicit RetryableCommand(const std::string& name, int failuresBeforeSuccess);

    /**
     * @brief ����������
     */
    ~RetryableCommand() override = default;

    /**
     * @brief ��������� �������
     */
    void execute() override;

    /**
     * @brief �������� ��� �������
     */
    const std::string& getName() const;

    /**
     * @brief �������� ���������� �������
     */
    int getAttemptCount() const;

    /**
     * @brief ���� �� ������� ��������� �������
     */
    bool isSuccessful() const;

    /**
     * @brief �������� ������� �������
     */
    void reset();

private:
    std::string name_;
    int failuresBeforeSuccess_;
    mutable std::atomic<int> attemptCount_;
    mutable std::atomic<bool> successful_;
};