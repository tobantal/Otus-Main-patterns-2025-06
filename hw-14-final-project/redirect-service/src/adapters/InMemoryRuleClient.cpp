#include "adapters/InMemoryRuleClient.hpp"
#include <iostream>

/**
 * @file InMemoryRuleClient.cpp
 * @brief Реализация заглушки клиента правил
 * @author Anton Tobolkin
 */

InMemoryRuleClient::InMemoryRuleClient()
{
    std::cout << "[InMemoryRuleClient] Initializing with hardcoded rules..." << std::endl;
    
    // Хардкод тестовых правил
    rules_["promo"] = Rule{"promo", "https://example.com/promo", ""};
    rules_["docs"] = Rule{"docs", "https://docs.example.com", ""};
    rules_["blog"] = Rule{"blog", "https://blog.example.com", ""};
    
    std::cout << "[InMemoryRuleClient] Loaded " << rules_.size() << " rules" << std::endl;
}

std::optional<Rule> InMemoryRuleClient::findByKey(const std::string& key)
{
    std::cout << "[InMemoryRuleClient] Looking for rule: " << key << std::endl;
    
    auto it = rules_.find(key);
    if (it != rules_.end())
    {
        std::cout << "[InMemoryRuleClient] Rule found: " << it->second.targetUrl << std::endl;
        return it->second;
    }
    
    std::cout << "[InMemoryRuleClient] Rule not found" << std::endl;
    return std::nullopt;
}