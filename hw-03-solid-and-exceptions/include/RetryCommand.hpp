#pragma once

#include "ICommand.hpp"
#include <memory>

/**
 * @file RetryCommand.hpp
 * @brief Команда для повторного выполнения другой команды
 * @author Space Battle Team
 * @version 1.0
 */

 /**
  * @brief Команда-декоратор для повторного выполнения другой команды
  *
  * Реализует паттерн Decorator для добавления возможности
  * повторного выполнения команды без изменения её кода.
  */
class RetryCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param command Команда для повторного выполнения
     */
    explicit RetryCommand(std::shared_ptr<ICommand> command);

    /**
     * @brief Деструктор
     */
    ~RetryCommand() override = default;

    /**
     * @brief Выполнить команду
     * @throws Пробрасывает исключение от внутренней команды
     */
    void execute() override;

    /**
     * @brief Получить внутреннюю команду
     * @return Указатель на внутреннюю команду
     */
    std::shared_ptr<ICommand> getInnerCommand() const;

private:
    /**
     * @brief Команда для повторного выполнения
     */
    std::shared_ptr<ICommand> command_;
};