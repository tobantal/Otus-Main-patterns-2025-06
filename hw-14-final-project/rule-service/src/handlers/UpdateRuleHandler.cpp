#include "handlers/UpdateRuleHandler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

/**
 * @file UpdateRuleHandler.cpp
 * @brief Реализация обработчика обновления правила
 * @author Anton Tobolkin
 */

UpdateRuleHandler::UpdateRuleHandler(
    std::shared_ptr<IRuleService> ruleService,
    std::shared_ptr<ICacheInvalidator> cacheInvalidator)
    : ruleService_(ruleService)
    , cacheInvalidator_(cacheInvalidator)
{
    std::cout << "[UpdateRuleHandler] Handler created" << std::endl;
}

void UpdateRuleHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[UpdateRuleHandler] Processing PUT /rules/{shortId}" << std::endl;
    
    try
    {
        // Извлекаем shortId из пути
        std::string shortId = extractShortId(req.getPath());
        
        if (shortId.empty())
        {
            res.setStatus(400);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Invalid shortId"})");
            return;
        }
        
        // Парсим JSON из body
        json body = json::parse(req.getBody());
        
        // Валидация обязательных полей
        if (!body.contains("targetUrl"))
        {
            res.setStatus(400);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Missing required field: targetUrl"})");
            return;
        }
        
        // Создаем объект Rule для обновления
        Rule rule{
            shortId,  // shortId не меняется
            body["targetUrl"].get<std::string>(),
            body.value("condition", "")
        };
        
        std::cout << "[UpdateRuleHandler] Updating rule: " << shortId << std::endl;
        
        // Обновляем правило
        bool updated = ruleService_->update(shortId, rule);
        
        if (!updated)
        {
            res.setStatus(404);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Rule not found"})");
            return;
        }
        
        // Инвалидируем кэш
        cacheInvalidator_->invalidate(shortId);
        
        // Возвращаем успех
        json response = {
            {"shortId", rule.shortId},
            {"targetUrl", rule.targetUrl},
            {"condition", rule.condition}
        };
        
        res.setStatus(200);
        res.setHeader("Content-Type", "application/json");
        res.setBody(response.dump());
        
        std::cout << "[UpdateRuleHandler] Rule updated successfully" << std::endl;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "[UpdateRuleHandler] JSON parse error: " << e.what() << std::endl;
        res.setStatus(400);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Invalid JSON"})");
    }
    catch (const std::exception& e)
    {
        std::cerr << "[UpdateRuleHandler] Error: " << e.what() << std::endl;
        res.setStatus(500);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Internal server error"})");
    }
}

std::string UpdateRuleHandler::extractShortId(const std::string& path) const
{
    // Ожидаем путь вида: /rules/promo
    std::string prefix = "/rules/";
    
    if (path.find(prefix) != 0)
    {
        return "";
    }
    
    std::string shortId = path.substr(prefix.length());
    
    // Убираем query string если есть
    size_t queryPos = shortId.find('?');
    if (queryPos != std::string::npos)
    {
        shortId = shortId.substr(0, queryPos);
    }
    
    return shortId;
}