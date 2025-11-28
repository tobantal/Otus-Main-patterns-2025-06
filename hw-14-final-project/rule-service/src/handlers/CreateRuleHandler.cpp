#include "handlers/CreateRuleHandler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

/**
 * @file CreateRuleHandler.cpp
 * @brief Реализация обработчика создания правила
 * @author Anton Tobolkin
 */

CreateRuleHandler::CreateRuleHandler(
    std::shared_ptr<IRuleService> ruleService,
    std::shared_ptr<ICacheInvalidator> cacheInvalidator)
    : ruleService_(ruleService)
    , cacheInvalidator_(cacheInvalidator)
{
    std::cout << "[CreateRuleHandler] Handler created" << std::endl;
}

void CreateRuleHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[CreateRuleHandler] Processing POST /rules" << std::endl;
    
    try
    {
        // Парсим JSON из body
        json body = json::parse(req.getBody());
        
        // Валидация обязательных полей
        if (!body.contains("shortId") || !body.contains("targetUrl"))
        {
            res.setStatus(400);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Missing required fields: shortId, targetUrl"})");
            return;
        }
        
        // Создаем объект Rule
        Rule rule{
            body["shortId"].get<std::string>(),
            body["targetUrl"].get<std::string>(),
            body.value("condition", "")  // condition опциональное
        };
        
        std::cout << "[CreateRuleHandler] Creating rule: " << rule.shortId << std::endl;
        
        // Создаем правило
        bool created = ruleService_->create(rule);
        
        if (!created)
        {
            res.setStatus(409);  // Conflict - уже существует
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Rule already exists"})");
            return;
        }
        
        // Инвалидируем кэш
        cacheInvalidator_->invalidate(rule.shortId);
        
        // Возвращаем успех
        json response = {
            {"shortId", rule.shortId},
            {"targetUrl", rule.targetUrl},
            {"condition", rule.condition}
        };
        
        res.setStatus(201);  // Created
        res.setHeader("Content-Type", "application/json");
        res.setBody(response.dump());
        
        std::cout << "[CreateRuleHandler] Rule created successfully" << std::endl;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "[CreateRuleHandler] JSON parse error: " << e.what() << std::endl;
        res.setStatus(400);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Invalid JSON"})");
    }
    catch (const std::exception& e)
    {
        std::cerr << "[CreateRuleHandler] Error: " << e.what() << std::endl;
        res.setStatus(500);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Internal server error"})");
    }
}