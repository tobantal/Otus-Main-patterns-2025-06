#include "NormalState.hpp"
#include <stdexcept>

/**
 * @file NormalState.cpp
 * @brief Реализация обычного состояния обработки команд
 * @author Anton Tobolkin
 * @version 1.0
 */

void NormalState::handle(std::shared_ptr<ICommand> command) {
    if (!command) {
        // Защита от nullptr
        return;
    }
    
    // В обычном состоянии просто выполняем команду
    // Команда сама решит, что делать (возможно изменит состояние)
    command->execute();
}
