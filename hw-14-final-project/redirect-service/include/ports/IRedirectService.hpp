#pragma once

#include "domain/RedirectRequest.hpp"
#include "domain/RedirectResult.hpp"

/**
 * @file IRedirectService.hpp
 * @brief Интерфейс порта для бизнес-логики редиректа
 * @author Anton Tobolkin
 */

/**
 * @class IRedirectService
 * @brief Порт для бизнес-логики переадресации
 */
class IRedirectService
{
public:
    virtual ~IRedirectService() = default;

    /**
     * @brief Выполнить переадресацию
     * @param req Запрос на редирект
     * @return Результат редиректа
     */
    virtual RedirectResult redirect(const RedirectRequest& req) = 0;
};