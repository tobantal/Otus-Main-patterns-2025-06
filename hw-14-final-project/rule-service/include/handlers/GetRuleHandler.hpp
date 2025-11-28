#pragma once
#include "IHttpHandler.hpp"
#include "ports/IRuleService.hpp"
#include <memory>

/**
 * @file GetRuleHandler.hpp
 * @brief Обработчик получения правила
 * @author Anton Tobolkin
 */

/**
 * @class GetRuleHandler
 * @brief Обрабатывает GET /rules/{shortId}
 */
class GetRuleHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    explicit GetRuleHandler(std::shared_ptr<IRuleService> ruleService);

    /**
     * @brief Обработать HTTP-запрос
     */
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<IRuleService> ruleService_;
    
    /**
     * @brief Извлечь shortId из пути /rules/{shortId}
     */
    std::string extractShortId(const std::string& path) const;
};