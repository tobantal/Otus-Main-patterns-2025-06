#include "LogCommand.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <typeinfo>

/**
 * @brief Конструктор с сообщением
 */
LogCommand::LogCommand(const std::string& message, const std::string& logFilePath)
    : message_(message), logFilePath_(logFilePath) {
}

/**
 * @brief Конструктор для логирования информации о команде и исключении
 */
LogCommand::LogCommand(std::shared_ptr<ICommand> failedCommand,
    const std::exception& exception,
    const std::string& logFilePath)
    : message_(formatErrorMessage(failedCommand, exception)),
    logFilePath_(logFilePath) {
}

/**
 * @brief Выполнить команду - записать сообщение в лог
 */
void LogCommand::execute() {
    // Открываем файл для добавления записей
    std::ofstream logFile(logFilePath_, std::ios::app);

    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + logFilePath_);
    }

    // Записываем сообщение с временной меткой
    logFile << "[" << getCurrentTime() << "] " << message_ << std::endl;
    
    logFile.close();

    // Также выводим в консоль для отладки
    std::cout << "[LOG] " << message_ << std::endl;
}

/**
 * @brief Получить сообщение лога
 */
const std::string& LogCommand::getMessage() const {
    return message_;
}

/**
 * @brief Получить текущее время в виде строки
 */
std::string LogCommand::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

/**
 * @brief Форматировать сообщение об ошибке
 */
std::string LogCommand::formatErrorMessage(std::shared_ptr<ICommand> command,
    const std::exception& exception) {
    std::stringstream ss;

    if (command) {
        // Получаем имя типа команды (может быть искажено компилятором)
        ss << "Command [" << typeid(*command).name() << "] ";
    }
    else {
        ss << "Command [Unknown] ";
    }

    ss << "failed with exception: " << exception.what();

    return ss.str();
}