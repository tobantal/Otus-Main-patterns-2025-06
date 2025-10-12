#include "SoftStopCommand.hpp"

SoftStopCommand::SoftStopCommand(std::shared_ptr<CommandExecutor> executor)
    : executor_(executor) {
    
    if (!executor) {
        throw std::invalid_argument("Executor cannot be null");
    }
}

void SoftStopCommand::execute() {
    executor_->softStop();
}