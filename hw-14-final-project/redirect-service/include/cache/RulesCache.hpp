#pragma once

#include "IRulesCache.hpp"
#include "ThreadSafeMap.hpp"
#include <memory>
#include <optional>
#include <iostream>
#include "domain/Rule.hpp"


/**
 * @brief Реализация потокобезопасного кэша правил
 */
class RulesCache : public IRulesCache
{
private:
    ThreadSafeMap<std::string, Rule> cache_;

public:
    RulesCache() = default;
    ~RulesCache() = default;

    std::optional<Rule> find(const std::string& id) override
    {
        auto rule = cache_.find(id);
        if (rule)
        {
            std::cout << "[RulesCache] Cache hit for rule: " << id << std::endl;
            return *rule;
        }
        std::cout << "[RulesCache] Cache miss for rule: " << id << std::endl;
        return std::nullopt;
    }

    void remove(const std::string& id) override
    {
        std::cout << "[RulesCache] Removing rule from cache: " << id << std::endl;
        cache_.remove(id);
    }

    void clear() override
    {
        std::cout << "[RulesCache] Clearing all cache" << std::endl;
        cache_.clear();
    }

    void put(const std::string& id, const Rule& rule) override
    {
        std::cout << "[RulesCache] Caching rule: " << id << std::endl;
        cache_.insert(id, std::make_shared<Rule>(rule));
    }
};
