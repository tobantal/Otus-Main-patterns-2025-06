#include "handlers/GetRuleHandler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

/**
 * @file GetRuleHandler.cpp
 * @brief Реализация обработчика получения правила
 * @author Anton Tobolkin
 */

GetRuleHandler::GetRuleHandler(std::shared_ptr<IRuleService> ruleService)
    : ruleService_(ruleService)
{
    std::cout << "[GetRuleHandler] Handler created" << std::endl;
}

void GetRuleHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[GetRuleHandler] Processing GET /rules/{shortId}" << std::endl;
    
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
        
        std::cout << "[GetRuleHandler] Looking for rule: " << shortId << std::endl;
        
        // Получаем правило
        auto rule = ruleService_->findById(shortId);
        
        if (!rule)
        {
            res.setStatus(404);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Rule not found"})");
            return;
        }
        
        // Формируем JSON ответ
        json response = {
            {"shortId", rule->shortId},
            {"targetUrl", rule->targetUrl},
            {"condition", rule->condition}
        };
        
        res.setStatus(200);
        res.setHeader("Content-Type", "application/json");
        res.setBody(response.dump());
        
        std::cout << "[GetRuleHandler] Rule found" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[GetRuleHandler] Error: " << e.what() << std::endl;
        res.setStatus(500);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Internal server error"})");
    }
}

std::string GetRuleHandler::extractShortId(const std::string& path) const
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