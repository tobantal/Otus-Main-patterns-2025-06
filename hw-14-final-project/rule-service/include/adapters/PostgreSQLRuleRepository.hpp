#pragma once

#include "ports/IRuleRepository.hpp"
#include "domain/Rule.hpp"
#include "domain/RuleEntity.hpp"
#include "domain/PaginatedRules.hpp"
#include <settings/IDbSettings.hpp>
#include <pqxx/pqxx>
#include <memory>
#include <string>

/**
 * @file PostgreSQLRuleRepository.hpp
 * @brief Адаптер для работы с PostgreSQL
 * @author Anton Tobolkin
 */

/**
 * @class PostgreSQLRuleRepository
 * @brief Реализация IRuleRepository для PostgreSQL
 */
class PostgreSQLRuleRepository : public IRuleRepository
{
public:
    /**
     * @brief Конструктор с параметрами подключения
     * @param connectionString Строка подключения PostgreSQL
     */
    explicit PostgreSQLRuleRepository(std::shared_ptr<IDbSettings> dbSettings);
    
    /**
     * @brief Деструктор
     */
    ~PostgreSQLRuleRepository() override;

    bool create(const Rule& rule) override;
    std::optional<Rule> findById(const std::string& shortId) override;
    PaginatedRules findAll(int page, int pageSize) override;
    bool update(const std::string& shortId, const Rule& rule) override;
    bool deleteById(const std::string& shortId) override;

private:
    std::unique_ptr<pqxx::connection> connection_;
    
    /**
     * @brief Конвертировать RuleEntity в Rule
     */
    Rule entityToRule(const RuleEntity& entity) const;
    
    /**
     * @brief Конвертировать Rule в RuleEntity
     */
    RuleEntity ruleToEntity(const Rule& rule) const;
};