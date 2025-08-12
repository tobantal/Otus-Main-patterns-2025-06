#include "RetryCommand.hpp"


/**
 * @brief Конструктор RetryCommand
 */
RetryCommand::RetryCommand(std::shared_ptr<ICommand> command)
    : command_(command) {
    if (!command) {
        throw std::invalid_argument("Command cannot be null");
    }
}

/**
 * @brief Выполнить команду
 */
void RetryCommand::execute() {
    // Просто делегируем выполнение внутренней команде
    // Если она выбросит исключение, оно будет проброшено дальше
    command_->execute();
}

/**
 * @brief Получить внутреннюю команду
 */
std::shared_ptr<ICommand> RetryCommand::getInnerCommand() const {
    return command_;
}