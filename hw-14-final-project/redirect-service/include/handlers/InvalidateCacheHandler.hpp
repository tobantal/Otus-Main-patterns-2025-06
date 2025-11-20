#pragma once

#include "IHttpHandler.hpp"
#include "cache/IRulesCache.hpp"
#include <memory>
#include <iostream>

/**
 * @brief Handler для полной инвалидации кэша
 * DELETE /cache/invalidate
 */
class InvalidateCacheHandler : public IHttpHandler
{
private:
    std::shared_ptr<IRulesCache> cache_;

public:
    explicit InvalidateCacheHandler(std::shared_ptr<IRulesCache> cache)
        : cache_(cache) {}

    void handle(IRequest& req, IResponse& res) override
    {
        std::cout << "[InvalidateCacheHandler] Clearing all cache" << std::endl;
        cache_->clear();
        res.setStatus(204); // No Content
        res.setBody("");
    }
};
