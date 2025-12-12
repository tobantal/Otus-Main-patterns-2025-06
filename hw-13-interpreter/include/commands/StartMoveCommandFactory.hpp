#pragma once

#include "ICommandFactory.hpp"
#include "commands/StartMoveCommand.hpp"

/**
 * @file StartMoveCommandFactory.hpp
 * @brief Фабрика команды начала движения
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Фабрика для создания StartMoveCommand
 */
class StartMoveCommandFactory : public ICommandFactory {
public:
    std::shared_ptr<ICommand> create(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params) override
    {
        return std::make_shared<StartMoveCommand>(object, params);
    }
};