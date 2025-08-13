#pragma once

#include "ICommand.hpp"
#include <string>
#include <memory>
#include <fstream>

/**
 * @file LogCommand.hpp
 * @brief ������� ��� ������ ���������� � ���
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief ������� ��� ������ ���������� �� ���������� � ���
  *
  * ��������� ������� Command ��� ����������� ����������.
  * ������������� �������� Single Responsibility - ������ �����������.
  */
class LogCommand : public ICommand {
public:
    /**
     * @brief �����������
     * @param message ��������� ��� ������ � ���
     * @param logFilePath ���� � ����� ���� (�� ��������� "error.log")
     */
    explicit LogCommand(const std::string& message,
        const std::string& logFilePath = "error.log");

    /**
     * @brief ����������� ��� ����������� ���������� � ������� � ����������
     * @param failedCommand �������, ������� ��������� ����������
     * @param exception ����������
     * @param logFilePath ���� � ����� ����
     */
    LogCommand(std::shared_ptr<ICommand> failedCommand,
        const std::exception& exception,
        const std::string& logFilePath = "error.log");

    /**
     * @brief ����������
     */
    ~LogCommand() override = default;

    /**
     * @brief ��������� ������� - �������� ��������� � ���
     */
    void execute() override;

    /**
     * @brief �������� ��������� ����
     * @return ��������� ��� �����������
     */
    const std::string& getMessage() const;

private:
    /**
     * @brief ��������� ��� ������ � ���
     */
    std::string message_;

    /**
     * @brief ���� � ����� ����
     */
    std::string logFilePath_;

    /**
     * @brief �������� ������� ����� � ���� ������
     * @return ������ � ������� ��������
     */
    static std::string getCurrentTime();

    /**
     * @brief ������������� ��������� �� ������
     * @param command �������
     * @param exception ����������
     * @return ����������������� ���������
     */
    static std::string formatErrorMessage(std::shared_ptr<ICommand> command,
        const std::exception& exception);
};