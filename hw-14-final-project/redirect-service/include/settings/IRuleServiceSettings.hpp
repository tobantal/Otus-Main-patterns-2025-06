#pragma once

#include <string>

/**
 * @file IRuleServiceSettings.hpp
 * @brief Интерфейс настроек rule-service
 * @author Anton Tobolkin
 */
class IRuleServiceSettings
{
public:
    virtual ~IRuleServiceSettings() = default;
    
    virtual std::string getUrl() const = 0;
};
