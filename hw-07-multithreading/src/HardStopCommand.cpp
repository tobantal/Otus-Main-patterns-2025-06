#include "HardStopCommand.hpp"
#include <stdexcept>

HardStopCommand::HardStopCommand(std::shared_ptr<CommandExecutor> executor)
    : executor_(executor) {
    
    if (!executor) {
        throw std::invalid_argument("Executor cannot be null");
    }
}

void HardStopCommand::execute() {
    executor_->hardStop();
}