#include "MoveToState.hpp"
#include <stdexcept>

/**
 * @file MoveToState.cpp
 * @brief Реализация состояния перенаправления команд
 * @author Anton Tobolkin
 * @version 1.0
 */

MoveToState::MoveToState(std::shared_ptr<ThreadSafeQueue> targetQueue)
    : targetQueue_(targetQueue) {
    
    if (!targetQueue) {
        throw std::invalid_argument("MoveToState: targetQueue cannot be nullptr");
    }
}

void MoveToState::handle(std::shared_ptr<ICommand> command) {
    if (!command) {
        // Защита от nullptr
        return;
    }
    
    // В состоянии MoveTo команды не выполняются,
    // а перенаправляются в целевую очередь
    // Это позволяет разгрузить текущий исполнитель
    // и передать обработку другому серверу/потоку
    targetQueue_->push(command);
    
    // Примечание: StateChangeCommand (HardStop, RunCommand) 
    // тоже попадут в targetQueue, но их execute() все равно
    // изменит состояние текущего исполнителя при выполнении
    // на другом сервере (если у них есть ссылка на этот executor)
}

std::shared_ptr<ThreadSafeQueue> MoveToState::getTargetQueue() const {
    return targetQueue_;
}
