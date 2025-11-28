#include "handlers/InvalidateCacheHandler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

/**
 * @file InvalidateCacheHandler.cpp
 * @brief Реализация обработчика инвалидации кэша
 * @author Anton Tobolkin
 */

InvalidateCacheHandler::InvalidateCacheHandler(std::shared_ptr<ICacheInvalidator> cacheInvalidator)
    : cacheInvalidator_(cacheInvalidator)
{
    std::cout << "[InvalidateCacheHandler] Handler created" << std::endl;
}

void InvalidateCacheHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[InvalidateCacheHandler] Processing cache invalidation" << std::endl;
    
    try
    {
        std::string path = req.getPath();
        
        // Проверяем, это запрос на полную инвалидацию или по shortId
        if (isInvalidateAll(path))
        {
            std::cout << "[InvalidateCacheHandler] Invalidating all cache" << std::endl;
            
            bool success = cacheInvalidator_->invalidateAll();
            
            if (success)
            {
                json response = {{"message", "All cache invalidated"}};
                res.setStatus(200);
                res.setHeader("Content-Type", "application/json");
                res.setBody(response.dump());
            }
            else
            {
                res.setStatus(500);
                res.setHeader("Content-Type", "application/json");
                res.setBody(R"({"error": "Failed to invalidate cache"})");
            }
        }
        else
        {
            // Извлекаем shortId
            std::string shortId = extractShortId(path);
            
            if (shortId.empty())
            {
                res.setStatus(400);
                res.setHeader("Content-Type", "application/json");
                res.setBody(R"({"error": "Invalid shortId"})");
                return;
            }
            
            std::cout << "[InvalidateCacheHandler] Invalidating cache for: " << shortId << std::endl;
            
            bool success = cacheInvalidator_->invalidate(shortId);
            
            if (success)
            {
                json response = {
                    {"message", "Cache invalidated"},
                    {"shortId", shortId}
                };
                res.setStatus(200);
                res.setHeader("Content-Type", "application/json");
                res.setBody(response.dump());
            }
            else
            {
                res.setStatus(500);
                res.setHeader("Content-Type", "application/json");
                res.setBody(R"({"error": "Failed to invalidate cache"})");
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[InvalidateCacheHandler] Error: " << e.what() << std::endl;
        res.setStatus(500);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Internal server error"})");
    }
}

bool InvalidateCacheHandler::isInvalidateAll(const std::string& path) const
{
    // Точное совпадение с /cache/invalidate (без trailing slash)
    return path == "/cache/invalidate";
}

std::string InvalidateCacheHandler::extractShortId(const std::string& path) const
{
    // Ожидаем путь вида: /cache/invalidate/promo
    std::string prefix = "/cache/invalidate/";
    
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