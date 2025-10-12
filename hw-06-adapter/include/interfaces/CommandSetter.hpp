#pragma once

#include "IGameObject.hpp"
#include "ICommand.hpp"
#include <memory>
#include <string>
#include <any>
#include <stdexcept>

/**
 * Команда стандартного set для IGameObject
 */
class CommandSetter : public ICommand
{
private:
    std::shared_ptr<IGameObject> m_object;
    std::string m_property;
    std::any m_value;

public:
    CommandSetter(std::shared_ptr<IGameObject> obj, const std::string &propertyName)
        : m_object(std::move(obj)), m_property(propertyName)
    {
        if (!m_object)
            throw std::invalid_argument("GameObject pointer is null");
    }

    void execute() override;

    void setValue(const std::any &value)
    {
        m_value = value;
    }
};
