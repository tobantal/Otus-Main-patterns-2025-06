#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <ICommand.hpp>

using json = nlohmann::json;

/**
 * @brief Интерфейс для создания команд с параметрами
 */
class ICommandBuilder {
public:
    virtual ~ICommandBuilder() = default;
    virtual std::shared_ptr<ICommand> build(const json& params) = 0;
};
