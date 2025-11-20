#include "services/RuleService.hpp"
#include <iostream>

/**
 * @file RuleService.cpp
 * @brief Реализация бизнес-логики управления правилами
 * @author Anton Tobolkin
 */

RuleService::RuleService(std::shared_ptr<IRuleRepository> repository)
    : repository_(repository)
{
    std::cout << "[RuleService] Service created" << std::endl;
}

bool RuleService::create(const Rule& rule)
{
    std::cout << "[RuleService] Creating rule: " << rule.shortId << std::endl;
    return repository_->create(rule);
}

std::optional<Rule> RuleService::findById(const std::string& shortId)
{
    std::cout << "[RuleService] Finding rule: " << shortId << std::endl;
    return repository_->findById(shortId);
}

PaginatedRules RuleService::findAll(int page, int pageSize)
{
    std::cout << "[RuleService] Listing rules, page=" << page << ", size=" << pageSize << std::endl;
    return repository_->findAll(page, pageSize);
}

bool RuleService::update(const std::string& shortId, const Rule& rule)
{
    std::cout << "[RuleService] Updating rule: " << shortId << std::endl;
    // FIXME: вот тут должен быть асинхронный вызов в redirect-service на сброс кэша
    //HttpCacheInvalidator::invalidate(shortId)
    return repository_->update(shortId, rule);
}

bool RuleService::deleteById(const std::string& shortId)
{
    std::cout << "[RuleService] Deleting rule: " << shortId << std::endl;
    return repository_->deleteById(shortId);
}