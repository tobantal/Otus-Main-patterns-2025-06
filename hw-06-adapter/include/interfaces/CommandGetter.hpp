#pragma once

#include "ICommand.hpp"
#include "interfaces/IGameObject.hpp"
#include <memory>
#include <string>
#include <any>
#include <stdexcept>

/**
 * Команда стандартного get для IGameObject
 */
class CommandGetter : public ICommand
{
private:
    std::shared_ptr<IGameObject> m_object;
    std::string m_property;
    std::any result_;

public:
    CommandGetter(std::shared_ptr<IGameObject> obj, const std::string &propertyName)
        : m_object(std::move(obj)), m_property(propertyName)
    {
        if (!m_object)
            throw std::invalid_argument("GameObject pointer is null");
    }

    void execute() override;

    std::any getResult() const
    {
        return result_;
    }
};
