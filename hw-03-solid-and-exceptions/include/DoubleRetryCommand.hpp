#pragma once

#include "ICommand.hpp"
#include <memory>


/**
 * @brief Команда для двойного повторения
 *
 * Используется для идентификации того, что команда
 * уже была повторена один раз.
 */
class DoubleRetryCommand : public ICommand {
public:
    /**
     * @brief Конструктор
     * @param command Команда для повторного выполнения
     */
    explicit DoubleRetryCommand(std::shared_ptr<ICommand> command);

    /**
     * @brief Деструктор
     */
    ~DoubleRetryCommand() override = default;

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