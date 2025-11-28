#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include "ICacheInvalidatorSettings.hpp"
#include <IEnvironment.hpp>

/**
 * @file CacheInvalidatorSettings.hpp
 * @brief Реализация настроек для сброса кэша в redirect-service
 * @author Anton Tobolkin
 */
class CacheInvalidatorSettings : public ICacheInvalidatorSettings {
private:
    std::string redirectServiceUrl_;

public:
    explicit CacheInvalidatorSettings(std::shared_ptr<IEnvironment> env) {
        try {
            redirectServiceUrl_ = env->get<std::string>("redirect_service.url");
        } catch (...) {
            throw std::runtime_error("Missing required setting: redirect_service.url");
        }
    }

    std::string getRedirectServiceUrl() const override {
        return redirectServiceUrl_;
    }
};