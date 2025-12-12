#include "commands/StopMoveCommand.hpp"
#include <stdexcept>

/**
 * @file StopMoveCommand.cpp
 * @brief Реализация команды остановки движения
 * @author Anton Tobolkin
 */

StopMoveCommand::StopMoveCommand(
    std::shared_ptr<IUObject> object,
    std::shared_ptr<IUObject> params)
    : object_(object)
{
    // params не используются, но принимаем для единообразия интерфейса
    (void)params;
    
    if (!object) {
        throw std::runtime_error("StopMoveCommand: object cannot be null");
    }
}

void StopMoveCommand::execute()
{
    // Останавливаем объект - устанавливаем скорость в 0
    object_->setProperty("velocity", 0);
}