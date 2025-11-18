#pragma once

#include "IHttpHandler.hpp"
#include "ports/IRedirectService.hpp"
#include <memory>

/**
 * @file RedirectHandler.hpp
 * @brief Хендлер для переадресации
 * @author Anton Tobolkin
 */

/**
 * @class RedirectHandler
 * @brief Обрабатывает GET запросы на /r/{shortId}
 */
class RedirectHandler : public IHttpHandler
{
public:
    explicit RedirectHandler(std::shared_ptr<IRedirectService> redirectService);
    
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<IRedirectService> redirectService_;
    
    /**
     * @brief Извлечь shortId из пути
     * @param path Путь запроса (например "/r/promo")
     * @return shortId или пустая строка
     */
    std::string extractShortId(const std::string& path) const;
};