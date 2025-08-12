#include "DoubleRetryCommand.hpp"


/**
 * @brief Конструктор DoubleRetryCommand
 */
DoubleRetryCommand::DoubleRetryCommand(std::shared_ptr<ICommand> command)
    : command_(command) {
    if (!command) {
        throw std::invalid_argument("Command cannot be null");
    }
}

/**
 * @brief Выполнить команду
 */
void DoubleRetryCommand::execute() {
    // Просто делегируем выполнение внутренней команде
    command_->execute();
}

/**
 * @brief Получить внутреннюю команду
 */
std::shared_ptr<ICommand> DoubleRetryCommand::getInnerCommand() const {
    return command_;
}