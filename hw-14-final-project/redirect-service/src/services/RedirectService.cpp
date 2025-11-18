#include "services/RedirectService.hpp"
#include <iostream>

/**
 * @file RedirectService.cpp
 * @brief Реализация сервиса переадресации
 * @author Anton Tobolkin
 */

RedirectService::RedirectService(
    std::shared_ptr<IRuleClient> ruleClient,
    std::shared_ptr<IRuleEvaluator> evaluator)
    : ruleClient_(ruleClient)
    , evaluator_(evaluator)
{
    std::cout << "[RedirectService] Service created with injected dependencies" << std::endl;
}

RedirectResult RedirectService::redirect(const RedirectRequest& req)
{
    std::cout << "[RedirectService] Processing redirect for: " << req.shortId << std::endl;
    
    // Получаем правило из клиента
    auto rule = ruleClient_->findByKey(req.shortId);
    
    if (!rule.has_value())
    {
        std::cout << "[RedirectService] Rule not found" << std::endl;
        return RedirectResult{false, "", "Rule not found for key: " + req.shortId};
    }
    
    // Проверяем активность правила
    if (!rule->isActive)
    {
        std::cout << "[RedirectService] Rule is inactive" << std::endl;
        return RedirectResult{false, "", "Rule is inactive"};
    }
    
    // Оцениваем DSL условие
    bool conditionMet = evaluator_->evaluate(rule->condition, req);
    
    if (!conditionMet)
    {
        std::cout << "[RedirectService] Condition not met" << std::endl;
        return RedirectResult{false, "", "Condition not satisfied"};
    }
    
    std::cout << "[RedirectService] Redirect successful to: " << rule->targetUrl << std::endl;
    
    return RedirectResult{true, rule->targetUrl, ""};
}