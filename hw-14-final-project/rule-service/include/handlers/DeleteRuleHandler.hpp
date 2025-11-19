#pragma once
#include "IHttpHandler.hpp"
#include "ports/IRuleService.hpp"
#include "ports/ICacheInvalidator.hpp"
#include <memory>

/**
 * @file DeleteRuleHandler.hpp
 * @brief Обработчик удаления правила
 * @author Anton Tobolkin
 */

/**
 * @class DeleteRuleHandler
 * @brief Обрабатывает DELETE /rules/{shortId}
 */
class DeleteRuleHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    DeleteRuleHandler(
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