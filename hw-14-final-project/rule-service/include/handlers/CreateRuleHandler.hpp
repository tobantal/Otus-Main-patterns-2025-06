#pragma once
#include "IHttpHandler.hpp"
#include "ports/IRuleService.hpp"
#include "ports/ICacheInvalidator.hpp"
#include <memory>

/**
 * @file CreateRuleHandler.hpp
 * @brief Обработчик создания правила
 * @author Anton Tobolkin
 */

/**
 * @class CreateRuleHandler
 * @brief Обрабатывает POST /rules
 */
class CreateRuleHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    CreateRuleHandler(
        std::shared_ptr<IRuleService> ruleService,
        std::shared_ptr<ICacheInvalidator> cacheInvalidator);

    /**
     * @brief Обработать HTTP-запрос
     */
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<IRuleService> ruleService_;
    std::shared_ptr<ICacheInvalidator> cacheInvalidator_;
};