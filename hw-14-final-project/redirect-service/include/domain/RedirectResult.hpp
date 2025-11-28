#pragma once

#include <string>

/**
 * @file RedirectResult.hpp
 * @brief Доменная сущность - результат редиректа
 * @author Anton Tobolkin
 */

/**
 * @struct RedirectResult
 * @brief Результат операции переадресации
 */
struct RedirectResult
{
    bool success;              ///< Успешно ли выполнен редирект
    std::string targetUrl;     ///< Целевой URL (если успешно)
    std::string errorMessage;  ///< Сообщение об ошибке (если неуспешно)
};