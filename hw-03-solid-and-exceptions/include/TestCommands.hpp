#pragma once

#include "ICommand.hpp"
#include <string>
#include <atomic>

/**
 * @file TestCommands.hpp
 * @brief Тестовые команды для демонстрации работы системы
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Простая команда, которая всегда выполняется успешно
  */
class SuccessCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param name Имя команды для идентификации
     */
    explicit SuccessCommand(const std::string& name);

    /**
     * @brief Деструктор
     */
    ~SuccessCommand() override = default;

    /**
     * @brief Выполнить команду
     */
    void execute() override;

    /**
     * @brief Получить имя команды
     */
    const std::string& getName() const;

    /**
     * @brief Получить количество выполнений
     */
    int getExecutionCount() const;

private:
    std::string name_;
    mutable std::atomic<int> executionCount_;
};

/**
 * @brief Команда, которая всегда выбрасывает исключение
 */
class FailCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param name Имя команды
     * @param errorMessage Сообщение об ошибке
     */
    explicit FailCommand(const std::string& name,
        const std::string& errorMessage = "Command failed");

    /**
     * @brief Деструктор
     */
    ~FailCommand() override = default;

    /**
     * @brief Выполнить команду - всегда выбрасывает исключение
     */
    void execute() override;

    /**
     * @brief Получить имя команды
     */
    const std::string& getName() const;

    /**
     * @brief Получить количество попыток выполнения
     */
    int getAttemptCount() const;

private:
    std::string name_;
    std::string errorMessage_;
    mutable std::atomic<int> attemptCount_;
};

/**
 * @brief Команда, которая выбрасывает исключение первые N раз
 */
class RetryableCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param name Имя команды
     * @param failuresBeforeSuccess Количество неудач перед успехом
     */
    explicit RetryableCommand(const std::string& name, int failuresBeforeSuccess);

    /**
     * @brief Деструктор
     */
    ~RetryableCommand() override = default;

    /**
     * @brief Выполнить команду
     */
    void execute() override;

    /**
     * @brief Получить имя команды
     */
    const std::string& getName() const;

    /**
     * @brief Получить количество попыток
     */
    int getAttemptCount() const;

    /**
     * @brief Была ли команда выполнена успешно
     */
    bool isSuccessful() const;

    /**
     * @brief Сбросить счетчик попыток
     */
    void reset();

private:
    std::string name_;
    int failuresBeforeSuccess_;
    mutable std::atomic<int> attemptCount_;
    mutable std::atomic<bool> successful_;
};