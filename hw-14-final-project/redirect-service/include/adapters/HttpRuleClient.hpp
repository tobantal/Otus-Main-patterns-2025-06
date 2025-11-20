#pragma once

#include "ports/IRuleClient.hpp"
#include "IHttpClient.hpp"
#include "../settings/IRuleServiceSettings.hpp"
#include <memory>

/**
 * @file HttpRuleClient.hpp
 * @brief HTTP клиент для получения правил от rule-service
 * @author Anton Tobolkin
 */
class HttpRuleClient : public IRuleClient
{
public:
    HttpRuleClient(std::shared_ptr<IHttpClient> httpClient,
                   std::shared_ptr<IRuleServiceSettings> settings);

    std::optional<Rule> findByKey(const std::string& key) override;

private:
    std::shared_ptr<IHttpClient> httpClient_;
    std::shared_ptr<IRuleServiceSettings> settings_;
};
