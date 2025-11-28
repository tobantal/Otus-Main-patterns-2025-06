#include "handlers/RedirectHandler.hpp"
#include "domain/RedirectRequest.hpp"
#include <iostream>

/**
 * @file RedirectHandler.cpp
 * @brief Реализация хендлера переадресации
 * @author Anton Tobolkin
 */

RedirectHandler::RedirectHandler(std::shared_ptr<IRedirectService> redirectService)
    : redirectService_(redirectService)
{
    std::cout << "[RedirectHandler] Handler created with injected service" << std::endl;
}

void RedirectHandler::handle(IRequest& req, IResponse& res)
{
    std::cout << "[RedirectHandler] Handling request: " << req.getMethod() << " " << req.getPath() << std::endl;
    
    // Извлекаем shortId из пути
    std::string shortId = extractShortId(req.getPath());
    
    if (shortId.empty())
    {
        std::cout << "[RedirectHandler] Invalid path - shortId not found" << std::endl;
        res.setStatus(400);
        res.setHeader("Content-Type", "text/plain");
        res.setBody("Bad Request: shortId is required");
        return;
    }
    
    std::cout << "[RedirectHandler] Extracted shortId: " << shortId << std::endl;
    
    // Строим запрос на редирект со всеми данными
    RedirectRequest redirectReq{
        shortId,
        req.getIp(),
        req.getHeaders()
    };
    
    // Логируем контекст для отладки
    std::cout << "[RedirectHandler] Client IP: " << redirectReq.ip << std::endl;
    auto uaIt = redirectReq.headers.find("User-Agent");
    if (uaIt != redirectReq.headers.end())
    {
        std::cout << "[RedirectHandler] User-Agent: " << uaIt->second << std::endl;
    }
    else
    {
        std::cout << "[RedirectHandler] User-Agent not found in headers" << std::endl;
    }
    
    // Вызываем сервис
    auto result = redirectService_->redirect(redirectReq);
    
    if (!result.success)
    {
        std::cout << "[RedirectHandler] Redirect failed: " << result.errorMessage << std::endl;
        res.setStatus(404);
        res.setHeader("Content-Type", "text/plain");
        res.setBody("Not Found: " + result.errorMessage);
        return;
    }
    
    std::cout << "[RedirectHandler] Redirecting to: " << result.targetUrl << std::endl;
    
    // Возвращаем HTTP 302 редирект
    res.setStatus(302);
    res.setHeader("Location", result.targetUrl);
    res.setBody("");
}

std::string RedirectHandler::extractShortId(const std::string& path) const
{
    // Ожидаем путь вида: /r/promo
    std::string prefix = "/r/";
    
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