#pragma once
#include "IHttpHandler.hpp"
#include "ports/ICacheInvalidator.hpp"
#include <memory>

/**
 * @file InvalidateCacheHandler.hpp
 * @brief Обработчик инвалидации кэша
 * @author Anton Tobolkin
 */

/**
 * @class InvalidateCacheHandler
 * @brief Обрабатывает GET /cache/invalidate и GET /cache/invalidate/{shortId}
 */
class InvalidateCacheHandler : public IHttpHandler
{
public:
    /**
     * @brief Конструктор с инъекцией зависимостей
     */
    explicit InvalidateCacheHandler(std::shared_ptr<ICacheInvalidator> cacheInvalidator);

    /**
     * @brief Обработать HTTP-запрос
     */
    void handle(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<ICacheInvalidator> cacheInvalidator_;
    
    /**
     * @brief Проверить, является ли запрос инвалидацией всего кэша
     */
    bool isInvalidateAll(const std::string& path) const;
    
    /**
     * @brief Извлечь shortId из пути /cache/invalidate/{shortId}
     */
    std::string extractShortId(const std::string& path) const;
};