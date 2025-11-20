#pragma once

#include <memory>
#include <string>
#include "IServerSettings.hpp"
#include "../IEnvironment.hpp"
#include "../exceptions/EnvironmentLoadException.hpp"

/**
 * @brief Реализация настроек сервера
 */
class ServerSettings : public IServerSettings {
private:
    std::string host_;
    int port_;

public:
    explicit ServerSettings(std::shared_ptr<IEnvironment> env) {
        try {
            host_ = env->get<std::string>("server.host");
        } catch (...) {
            throw EnvironmentLoadException("Missing required setting: server.host");
        }

        try {
            port_ = env->get<int>("server.port");
        } catch (...) {
            throw EnvironmentLoadException("Missing required setting: server.port");
        }
    }

    std::string getHost() const override {
        return host_;
    }

    int getPort() const override {
        return port_;
    }
};