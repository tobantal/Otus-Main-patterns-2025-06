#include "DoubleRetryCommand.hpp"


/**
 * @brief ����������� DoubleRetryCommand
 */
DoubleRetryCommand::DoubleRetryCommand(std::shared_ptr<ICommand> command)
    : command_(command) {
    if (!command) {
        throw std::invalid_argument("Command cannot be null");
    }
}

/**
 * @brief ��������� �������
 */
void DoubleRetryCommand::execute() {
    // ������ ���������� ���������� ���������� �������
    command_->execute();
}

/**
 * @brief �������� ���������� �������
 */
std::shared_ptr<ICommand> DoubleRetryCommand::getInnerCommand() const {
    return command_;
}