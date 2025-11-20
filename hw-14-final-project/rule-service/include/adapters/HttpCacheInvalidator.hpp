#pragma once

#include "ports/ICacheInvalidator.hpp"
#include "../settings/ICacheInvalidatorSettings.hpp"
#include <memory>
#include <string>

/**
 * @file HttpCacheInvalidator.hpp
 * @brief Адаптер для отправки HTTP-запросов инвалидации кэша
 * @author Anton Tobolkin
 */

/**
 * @class HttpCacheInvalidator
 * @brief Реализация ICacheInvalidator через HTTP
 */
class HttpCacheInvalidator : public ICacheInvalidator
{
public:
    /**
     * @brief Конструктор с URL redirect-service
     * @param redirectServiceUrl Базовый URL redirect-service (например, http://localhost:8080)
     */
    explicit HttpCacheInvalidator(std::shared_ptr<ICacheInvalidatorSettings> settings);

    bool invalidate(const std::string& shortId) override;
    bool invalidateAll() override;

private:
    std::string redirectServiceUrl_;
    
    /**
     * @brief Отправить GET-запрос на указанный путь
     * @param path Путь для запроса (например, /cache/invalidate/promo)
     * @return true если запрос успешен
     */
    bool sendGetRequest(const std::string& path) const;
};