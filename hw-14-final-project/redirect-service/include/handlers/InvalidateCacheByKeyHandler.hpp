#pragma once

#include "IHttpHandler.hpp"
#include "cache/IRulesCache.hpp"
#include <memory>
#include <iostream>

/**
 * @brief Handler для инвалидации конкретного правила
 * DELETE /cache/invalidate/{ruleId}
 */
class InvalidateCacheByKeyHandler : public IHttpHandler
{
private:
    std::shared_ptr<IRulesCache> cache_;

public:
    explicit InvalidateCacheByKeyHandler(std::shared_ptr<IRulesCache> cache)
        : cache_(cache) {}

    void handle(IRequest& req, IResponse& res) override
    {
        // Извлекаем ruleId из пути: /cache/invalidate/{ruleId}
        std::string path = req.getPath();
        std::string ruleId = path.substr(path.find_last_of('/') + 1);
        
        std::cout << "[InvalidateCacheByKeyHandler] Removing rule from cache: " << ruleId << std::endl;
        cache_->remove(ruleId);
        res.setStatus(204); // No Content
        res.setBody("");
    }
};
