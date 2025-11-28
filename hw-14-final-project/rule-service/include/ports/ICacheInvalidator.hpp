#pragma once
#include <string>

/**
 * @file ICacheInvalidator.hpp
 * @brief Интерфейс для инвалидации кэша в redirect-service
 * @author Anton Tobolkin
 */

/**
 * @interface ICacheInvalidator
 * @brief Порт для отправки команд инвалидации кэша
 */
class ICacheInvalidator
{
public:
    virtual ~ICacheInvalidator() = default;

    /**
     * @brief Инвалидировать кэш для конкретного shortId
     * @param shortId Короткий идентификатор правила
     * @return true если успешно отправлено
     */
    virtual bool invalidate(const std::string& shortId) = 0;

    /**
     * @brief Инвалидировать весь кэш
     * @return true если успешно отправлено
     */
    virtual bool invalidateAll() = 0;
};