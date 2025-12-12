#pragma once

#include "ICommandFactory.hpp"
#include "commands/StopMoveCommand.hpp"

/**
 * @file StopMoveCommandFactory.hpp
 * @brief Фабрика команды остановки движения
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Фабрика для создания StopMoveCommand
 */
class StopMoveCommandFactory : public ICommandFactory {
public:
    std::shared_ptr<ICommand> create(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params) override
    {
        return std::make_shared<StopMoveCommand>(object, params);
    }
};