#pragma once

#include "ports/IRuleRepository.hpp"
#include "domain/Rule.hpp"
#include "domain/PaginatedRules.hpp"
#include <ThreadSafeMap.hpp>
#include <memory>
#include <string>
#include <iostream>

/**
 * @file InMemoryRuleRepository.hpp
 * @brief In-memory заглушка репозитория правил
 * @author Anton Tobolkin
 */

/**
 * @class InMemoryRuleRepository
 * @brief Реализация IRuleRepository в памяти (для тестирования без БД)
 */
class InMemoryRuleRepository : public IRuleRepository
{
public:
    InMemoryRuleRepository();
    ~InMemoryRuleRepository() override;

    bool create(const Rule& rule) override;
    std::optional<Rule> findById(const std::string& shortId) override;
    PaginatedRules findAll(int page, int pageSize) override;
    bool update(const std::string& shortId, const Rule& rule) override;
    bool deleteById(const std::string& shortId) override;

private:
    // Потокобезопасное хранилище правил: shortId → Rule
    ThreadSafeMap<std::string, Rule> rules_;
    
    // Счётчик для отслеживания всех когда-либо созданных правил
    int totalCount_ = 0;
};
