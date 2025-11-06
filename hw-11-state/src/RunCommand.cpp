#include "RunCommand.hpp"
#include "NormalState.hpp"

/**
 * @file RunCommand.cpp
 * @brief Реализация команды возврата в обычное состояние
 * @author Anton Tobolkin
 * @version 1.0
 */

RunCommand::RunCommand(std::shared_ptr<CommandExecutor> executor)
    : StateChangeCommand(executor, std::make_shared<NormalState>()) {
    
    // Создаем NormalState и передаем его в базовый класс
    // При выполнении execute() исполнитель вернется
    // в нормальный режим обработки команд
}
