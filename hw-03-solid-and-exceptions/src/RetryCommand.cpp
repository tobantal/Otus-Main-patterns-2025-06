#include "RetryCommand.hpp"


/**
 * @brief ����������� RetryCommand
 */
RetryCommand::RetryCommand(std::shared_ptr<ICommand> command)
    : command_(command) {
    if (!command) {
        throw std::invalid_argument("Command cannot be null");
    }
}

/**
 * @brief ��������� �������
 */
void RetryCommand::execute() {
    // ������ ���������� ���������� ���������� �������
    // ���� ��� �������� ����������, ��� ����� ���������� ������
    command_->execute();
}

/**
 * @brief �������� ���������� �������
 */
std::shared_ptr<ICommand> RetryCommand::getInnerCommand() const {
    return command_;
}