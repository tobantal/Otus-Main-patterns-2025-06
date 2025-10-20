#pragma once

#include "game_manager.hpp"
#include "IocContainer.hpp"
#include <nlohmann/json.hpp>

class InterpretCommand {
public:
    explicit InterpretCommand(GameManager& manager);
    void execute();

private:
    GameManager& manager_;
    IocContainer& container_;
};
