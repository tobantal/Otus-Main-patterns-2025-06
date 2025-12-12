#include "commands/StartMoveCommand.hpp"
#include <stdexcept>

/**
 * @file StartMoveCommand.cpp
 * @brief Реализация команды начала движения
 * @author Anton Tobolkin
 */

StartMoveCommand::StartMoveCommand(
    std::shared_ptr<IUObject> object,
    std::shared_ptr<IUObject> params)
    : object_(object)
    , initialVelocity_(0)
{
    if (!object) {
        throw std::runtime_error("StartMoveCommand: object cannot be null");
    }
    
    // Получаем параметр initialVelocity
    if (params) {
        try {
            initialVelocity_ = std::any_cast<int>(params->getProperty("initialVelocity"));
        } catch (const std::bad_any_cast& e) {
            throw std::runtime_error("StartMoveCommand: 'initialVelocity' must be an integer");
        } catch (const std::exception& e) {
            throw std::runtime_error("StartMoveCommand requires 'initialVelocity' parameter");
        }
    } else {
        throw std::runtime_error("StartMoveCommand requires parameters with 'initialVelocity'");
    }
}

void StartMoveCommand::execute()
{
    // Устанавливаем скорость объекта
    object_->setProperty("velocity", initialVelocity_);
}