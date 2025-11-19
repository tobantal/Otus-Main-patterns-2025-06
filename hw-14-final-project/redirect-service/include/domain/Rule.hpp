#pragma once

#include <string>

/**
 * @file Rule.hpp
 * @brief Доменная сущность - правило редиректа
 * @author Anton Tobolkin
 */

/**
 * @struct Rule
 * @brief Правило переадресации
 */
struct Rule
{
    std::string key;           ///< Короткий ID (например "promo", "docs")
    std::string targetUrl;     ///< Целевой URL для редиректа
    std::string condition;     ///< DSL условие (например "browser == chrome")
};