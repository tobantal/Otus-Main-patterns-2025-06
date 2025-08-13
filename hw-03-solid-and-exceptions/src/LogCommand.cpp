#include "LogCommand.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <typeinfo>

/**
 * @brief ����������� � ����������
 */
LogCommand::LogCommand(const std::string& message, const std::string& logFilePath)
    : message_(message), logFilePath_(logFilePath) {
}

/**
 * @brief ����������� ��� ����������� ���������� � ������� � ����������
 */
LogCommand::LogCommand(std::shared_ptr<ICommand> failedCommand,
    const std::exception& exception,
    const std::string& logFilePath)
    : message_(formatErrorMessage(failedCommand, exception)),
    logFilePath_(logFilePath) {
}

/**
 * @brief ��������� ������� - �������� ��������� � ���
 */
void LogCommand::execute() {
    // ��������� ���� ��� ���������� �������
    std::ofstream logFile(logFilePath_, std::ios::app);

    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + logFilePath_);
    }

    // ���������� ��������� � ��������� ������
    logFile << "[" << getCurrentTime() << "] " << message_ << std::endl;
    
    logFile.close();

    // ����� ������� � ������� ��� �������
    std::cout << "[LOG] " << message_ << std::endl;
}

/**
 * @brief �������� ��������� ����
 */
const std::string& LogCommand::getMessage() const {
    return message_;
}

/**
 * @brief �������� ������� ����� � ���� ������
 */
std::string LogCommand::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

/**
 * @brief ������������� ��������� �� ������
 */
std::string LogCommand::formatErrorMessage(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    std::stringstream ss;

    if (command) {
        // �������� ��� ���� ������� (����� ���� �������� ������������)
        ss << "Command [" << typeid(*command).name() << "] ";
    }
    else {
        ss << "Command [Unknown] ";
    }

    ss << "failed with exception: " << exception.what();

    return ss.str();
}