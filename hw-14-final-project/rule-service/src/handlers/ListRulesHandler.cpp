#include "handlers/ListRulesHandler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

/**
 * @file ListRulesHandler.cpp
 * @brief Реализация обработчика получения списка правил
 * @author Anton Tobolkin
 */

ListRulesHandler::ListRulesHandler(std::shared_ptr<IRuleService> ruleService)
    : ruleService_(ruleService)
{
    std::cout << "[ListRulesHandler] Handler created" << std::endl;
}

void ListRulesHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[ListRulesHandler] Processing GET /rules" << std::endl;
    
    try
    {
        // Получаем параметры пагинации из query string
        auto params = req.getParams();
        
        int page = 1;
        int pageSize = 10;
        
        if (params.count("page"))
        {
            page = std::stoi(params.at("page"));
        }
        
        if (params.count("size"))
        {
            pageSize = std::stoi(params.at("size"));
        }
        
        // Валидация параметров
        if (page < 1 || pageSize < 1 || pageSize > 100)
        {
            res.setStatus(400);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Invalid pagination parameters"})");
            return;
        }
        
        std::cout << "[ListRulesHandler] Fetching page=" << page << ", size=" << pageSize << std::endl;
        
        // Получаем список правил
        auto result = ruleService_->findAll(page, pageSize);
        
        // Формируем JSON ответ
        json rulesArray = json::array();
        for (const auto& rule : result.rules)
        {
            rulesArray.push_back({
                {"shortId", rule.shortId},
                {"targetUrl", rule.targetUrl},
                {"condition", rule.condition}
            });
        }
        
        json response = {
            {"rules", rulesArray},
            {"totalCount", result.totalCount},
            {"page", result.page},
            {"pageSize", result.pageSize}
        };
        
        res.setStatus(200);
        res.setHeader("Content-Type", "application/json");
        res.setBody(response.dump());
        
        std::cout << "[ListRulesHandler] Returned " << result.rules.size() << " rules" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ListRulesHandler] Error: " << e.what() << std::endl;
        res.setStatus(500);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Internal server error"})");
    }
}