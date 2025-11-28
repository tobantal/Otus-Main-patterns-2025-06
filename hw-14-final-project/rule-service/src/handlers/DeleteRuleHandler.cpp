#include "handlers/DeleteRuleHandler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

/**
 * @file DeleteRuleHandler.cpp
 * @brief Реализация обработчика удаления правила
 * @author Anton Tobolkin
 */

DeleteRuleHandler::DeleteRuleHandler(
    std::shared_ptr<IRuleService> ruleService,
    std::shared_ptr<ICacheInvalidator> cacheInvalidator)
    : ruleService_(ruleService)
    , cacheInvalidator_(cacheInvalidator)
{
    std::cout << "[DeleteRuleHandler] Handler created" << std::endl;
}

void DeleteRuleHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[DeleteRuleHandler] Processing DELETE /rules/{shortId}" << std::endl;
    
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
        
        std::cout << "[DeleteRuleHandler] Deleting rule: " << shortId << std::endl;
        
        // Удаляем правило
        bool deleted = ruleService_->deleteById(shortId);
        
        if (!deleted)
        {
            res.setStatus(404);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Rule not found"})");
            return;
        }
        
        // Инвалидируем кэш
        cacheInvalidator_->invalidate(shortId);
        
        // Возвращаем успех (204 No Content)
        res.setStatus(204);
        res.setBody("");
        
        std::cout << "[DeleteRuleHandler] Rule deleted successfully" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[DeleteRuleHandler] Error: " << e.what() << std::endl;
        res.setStatus(500);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Internal server error"})");
    }
}

std::string DeleteRuleHandler::extractShortId(const std::string& path) const
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