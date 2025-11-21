#pragma once
#include <string>

/**
 * @file Rule.hpp
 * @brief Доменная сущность - правило редиректа
 * @author Anton Tobolkin
 */

/**
 * @struct Rule
 * @brief Правило переадресации с условием
 */
struct Rule
{
    std::string shortId;     ///< Короткий ID (PRIMARY KEY)
    std::string targetUrl;   ///< Целевой URL для редиректа
    std::string condition;   ///< DSL-условие активации правила
};

inline bool operator==(const Rule& a, const Rule& b)
{
    return std::tie(a.shortId, a.targetUrl, a.condition) ==
           std::tie(b.shortId, b.targetUrl, b.condition);
}

inline bool operator!=(const Rule& a, const Rule& b)
{
    return !(a == b);
}