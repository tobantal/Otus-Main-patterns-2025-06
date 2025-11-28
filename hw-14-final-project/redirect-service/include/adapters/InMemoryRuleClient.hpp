#pragma once

#include "ports/IRuleClient.hpp"
#include "domain/Rule.hpp"
#include <map>
#include <string>
#include <optional>

/**
 * @file InMemoryRuleClient.hpp
 * @brief Заглушка - хранилище правил в памяти
 * @author Anton Tobolkin
 */

/**
 * @class InMemoryRuleClient
 * @brief In-memory реализация клиента правил (для тестирования)
 */
class InMemoryRuleClient : public IRuleClient
{
public:
    InMemoryRuleClient();
    
    std::optional<Rule> findByKey(const std::string& key) override;

private:
    std::map<std::string, Rule> rules_;
};