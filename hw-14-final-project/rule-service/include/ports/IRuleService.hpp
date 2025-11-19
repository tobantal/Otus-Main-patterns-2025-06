#pragma once
#include "domain/Rule.hpp"
#include "domain/PaginatedRules.hpp"
#include <string>
#include <optional>

/**
 * @file IRuleService.hpp
 * @brief Интерфейс сервиса управления правилами
 * @author Anton Tobolkin
 */

/**
 * @interface IRuleService
 * @brief Интерфейс для бизнес-логики управления правилами
 */
class IRuleService
{
public:
    virtual ~IRuleService() = default;
    
    virtual bool create(const Rule& rule) = 0;
    virtual std::optional<Rule> findById(const std::string& shortId) = 0;
    virtual PaginatedRules findAll(int page, int pageSize) = 0;
    virtual bool update(const std::string& shortId, const Rule& rule) = 0;
    virtual bool deleteById(const std::string& shortId) = 0;
};