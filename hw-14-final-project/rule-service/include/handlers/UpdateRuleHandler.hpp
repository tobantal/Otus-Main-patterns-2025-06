#pragma once
#include "IHttpHandler.hpp"
#include "ports/IRuleService.hpp"
#include "ports/ICacheInvalidator.hpp"
#include <memory>

/**
 * @file UpdateRuleHandler.hpp
 * @brief Обработчик обновления правила
 * @author Anton Tobolkin
 */

/**
 * @class UpdateRuleHandler
 * @brief Обрабатывает PUT /rules/{shortId}
 */
class UpdateRuleHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    UpdateRuleHandler(
        std::shared_ptr<IRuleService> ruleService,
        std::shared_ptr<ICacheInvalidator> cacheInvalidator);

    /**
     * @brief Обработать HTTP-запрос
     */
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<IRuleService> ruleService_;
    std::shared_ptr<ICacheInvalidator> cacheInvalidator_;
    
    /**
     * @brief Извлечь shortId из пути /rules/{shortId}
     */
    std::string extractShortId(const std::string& path) const;
};