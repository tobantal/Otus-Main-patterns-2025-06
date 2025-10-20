#include "StartCommand.hpp"
#include <stdexcept>

StartCommand::StartCommand(std::shared_ptr<CommandExecutor> executor)
    : executor_(executor) {
    
    if (!executor) {
        throw std::invalid_argument("Executor cannot be null");
    }
}

void StartCommand::execute() {
    executor_->start();
}