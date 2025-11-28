#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include "settings/IRuleServiceSettings.hpp"
#include "IEnvironment.hpp"

class RuleServiceSettings : public IRuleServiceSettings
{
private:
    std::string url_;

public:
    explicit RuleServiceSettings(std::shared_ptr<IEnvironment> env)
    {
        try
        {
            std::cout << "[RuleServiceSettings] Looking for services.rule_service_url" << std::endl;
            url_ = env->get<std::string>("services.rule_service_url");
            std::cout << "[RuleServiceSettings] Got URL: " << url_ << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "[RuleServiceSettings] Error: " << e.what() << std::endl;
            throw std::runtime_error("Missing required setting: services.rule_service_url");
        }
    }

    std::string getUrl() const override
    {
        return url_;
    }
};
