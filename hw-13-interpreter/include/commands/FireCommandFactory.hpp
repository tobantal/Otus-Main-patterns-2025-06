#pragma once

#include "ICommandFactory.hpp"
#include "commands/FireCommand.hpp"

/**
 * @file FireCommandFactory.hpp
 * @brief Фабрика команды выстрела
 * @author Anton Tobolkin
 * @version 1.0
 */

/**
 * @brief Фабрика для создания FireCommand
 */
class FireCommandFactory : public ICommandFactory {
public:
    std::shared_ptr<ICommand> create(
        std::shared_ptr<IUObject> object,
        std::shared_ptr<IUObject> params) override
    {
        return std::make_shared<FireCommand>(object, params);
    }
};