#pragma once

#include <string>
#include <map>

/**
 * @file RedirectRequest.hpp
 * @brief Доменная сущность - запрос на редирект
 * @author Anton Tobolkin
 */

/**
 * @struct RedirectRequest
 * @brief Запрос на переадресацию
 * 
 * Содержит контекст запроса для оценки DSL условий.
 */
struct RedirectRequest
{
    std::string shortId;                        ///< Короткий ID из URL (/r/{shortId})
    std::string ip;                             ///< IP адрес клиента
    std::map<std::string, std::string> headers; ///< HTTP заголовки (включая User-Agent)
};