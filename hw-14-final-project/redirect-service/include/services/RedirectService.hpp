#pragma once

#include "ports/IRedirectService.hpp"
#include "ports/IRuleClient.hpp"
#include "ports/IRuleEvaluator.hpp"
#include <memory>

/**
 * @file RedirectService.hpp
 * @brief Реализация бизнес-логики редиректа
 * @author Anton Tobolkin
 */

/**
 * @class RedirectService
 * @brief Сервис переадресации
 * 
 * Получает правило из IRuleClient, проверяет условие через IRuleEvaluator,
 * возвращает целевой URL.
 */
class RedirectService : public IRedirectService
{
public:
    RedirectService(
        std::shared_ptr<IRuleClient> ruleClient,
        std::shared_ptr<IRuleEvaluator> evaluator
    );
    
    RedirectResult redirect(const RedirectRequest& req) override;

private:
    std::shared_ptr<IRuleClient> ruleClient_;
    std::shared_ptr<IRuleEvaluator> evaluator_;
};