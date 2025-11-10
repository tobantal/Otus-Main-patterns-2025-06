#include "MoveToCommand.hpp"
#include "MoveToState.hpp"
#include <stdexcept>

/**
 * @file MoveToCommand.cpp
 * @brief Реализация команды перехода в состояние перенаправления
 * @author Anton Tobolkin
 * @version 1.0
 */

MoveToCommand::MoveToCommand(
    std::shared_ptr<CommandExecutor> executor,
    std::shared_ptr<ThreadSafeQueue> targetQueue)
    : StateChangeCommand(executor, std::make_shared<MoveToState>(targetQueue)),
      targetQueue_(targetQueue) {
    
    if (!targetQueue) {
        throw std::invalid_argument("MoveToCommand: targetQueue cannot be nullptr");
    }
    
    // Создаем MoveToState с указанной очередью
    // и передаем его в базовый класс как targetState_
}

std::shared_ptr<ThreadSafeQueue> MoveToCommand::getTargetQueue() const {
    return targetQueue_;
}
