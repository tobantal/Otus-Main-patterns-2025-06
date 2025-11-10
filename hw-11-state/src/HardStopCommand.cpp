#include "HardStopCommand.hpp"

HardStopCommand::HardStopCommand(std::shared_ptr<CommandExecutor> executor)
    : StateChangeCommand(executor, nullptr) {
        // остановка обработки
}