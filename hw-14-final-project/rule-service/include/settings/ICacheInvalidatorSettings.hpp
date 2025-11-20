#pragma once

#include <string>

/**
 * @brief Интерфейс настроек для сброса кэша в redirect-service. 
 */
class ICacheInvalidatorSettings {
public:
    virtual ~ICacheInvalidatorSettings() = default;
    virtual std::string getRedirectServiceUrl() const = 0;
};