#pragma once
#include "IHttpHandler.hpp"
#include "ports/IRuleService.hpp"
#include <memory>

/**
 * @file ListRulesHandler.hpp
 * @brief Обработчик получения списка правил
 * @author Anton Tobolkin
 */

/**
 * @class ListRulesHandler
 * @brief Обрабатывает GET /rules
 */
class ListRulesHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    explicit ListRulesHandler(std::shared_ptr<IRuleService> ruleService);

    /**
     * @brief Обработать HTTP-запрос
     */
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<IRuleService> ruleService_;
};