#pragma once

#include <memory>
#include <optional>
#include "ports/IRuleClient.hpp"
#include "HttpClient.hpp"
#include "settings/IRuleServiceSettings.hpp"
#include "cache/IRulesCache.hpp"

/**
 * @file HttpRuleClient.hpp
 * @brief HTTP клиент для получения правил от rule-service
 * @author Anton Tobolkin
 */
class HttpRuleClient : public IRuleClient
{
private:
    std::shared_ptr<IHttpClient> httpClient_;
    std::shared_ptr<IRuleServiceSettings> settings_;
    std::shared_ptr<IRulesCache> cache_;

public:
    HttpRuleClient(std::shared_ptr<IHttpClient> httpClient,
                   std::shared_ptr<IRuleServiceSettings> settings,
                   std::shared_ptr<IRulesCache> cache);

    std::optional<Rule> findByKey(const std::string &key) override;
};
