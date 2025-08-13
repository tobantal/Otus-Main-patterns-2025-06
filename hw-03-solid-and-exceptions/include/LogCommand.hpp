#pragma once

#include "ICommand.hpp"
#include <string>
#include <memory>
#include <fstream>

/**
 * @file LogCommand.hpp
 * @brief Команда для записи информации в лог
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда для записи информации об исключении в лог
  *
  * Реализует паттерн Command для логирования исключений.
  * Соответствует принципу Single Responsibility - только логирование.
  */
class LogCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param message Сообщение для записи в лог
     * @param logFilePath Путь к файлу лога (по умолчанию "error.log")
     */
    explicit LogCommand(const std::string& message,
        const std::string& logFilePath = "error.log");

    /**
     * @brief Конструктор для логирования информации о команде и исключении
     * @param failedCommand Команда, которая выбросила исключение
     * @param exception Исключение
     * @param logFilePath Путь к файлу лога
     */
    LogCommand(std::shared_ptr<ICommand> failedCommand,
        const std::exception& exception,
        const std::string& logFilePath = "error.log");

    /**
     * @brief Деструктор
     */
    ~LogCommand() override = default;

    /**
     * @brief Выполнить команду - записать сообщение в лог
     */
    void execute() override;

    /**
     * @brief Получить сообщение лога
     * @return Сообщение для логирования
     */
    const std::string& getMessage() const;

private:
    /**
     * @brief Сообщение для записи в лог
     */
    std::string message_;

    /**
     * @brief Путь к файлу лога
     */
    std::string logFilePath_;

    /**
     * @brief Получить текущее время в виде строки
     * @return Строка с текущим временем
     */
    static std::string getCurrentTime();

    /**
     * @brief Форматировать сообщение об ошибке
     * @param command Команда
     * @param exception Исключение
     * @return Отформатированное сообщение
     */
    static std::string formatErrorMessage(std::shared_ptr<ICommand> command,
        const std::exception& exception);
};