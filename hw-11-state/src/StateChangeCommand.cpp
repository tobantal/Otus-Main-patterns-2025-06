#include "StateChangeCommand.hpp"
#include "CommandExecutor.hpp"
#include <stdexcept>

/**
 * @file StateChangeCommand.cpp
 * @brief Реализация базового класса для команд смены состояния
 * @author Anton Tobolkin
 * @version 1.0
 */

StateChangeCommand::StateChangeCommand(
    std::shared_ptr<CommandExecutor> executor,
    std::shared_ptr<IExecutorState> targetState)
    : executor_(executor), targetState_(targetState) {
    
    if (!executor) {
        throw std::invalid_argument("StateChangeCommand: executor cannot be nullptr");
    }
    
    // targetState может быть nullptr (для команды остановки)
}

void StateChangeCommand::execute() {
    // Единая логика для всех команд смены состояния:
    // устанавливаем новое состояние в исполнителе
    executor_->setState(targetState_);
    
    // Примечание: если targetState_ == nullptr,
    // это приведет к остановке потока исполнителя
}

std::shared_ptr<IExecutorState> StateChangeCommand::getTargetState() const {
    return targetState_;
}
