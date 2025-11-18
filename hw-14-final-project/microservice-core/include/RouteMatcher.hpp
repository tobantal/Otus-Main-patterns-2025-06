#pragma once

#include <string>
#include <vector>

/**
 * @file RouteMatcher.hpp
 * @brief Утилита для сопоставления путей с паттернами
 * @author Anton Tobolkin
 */

/**
 * @class RouteMatcher
 * @brief Проверяет соответствие пути паттерну с wildcard (*)
 * 
 * Паттерн: "GET:/r/*" означает один сегмент после /r
 * Примеры:
 * - "/r/promo" matches "/r/*"
 * - "/r/abc/123" не matches "/r/*"
 */
class RouteMatcher
{
public:
    /**
     * @brief Проверить соответствие пути паттерну
     * @param pattern Паттерн с wildcard (например "/r/*")
     * @param path Путь для проверки (например "/r/promo")
     * @return true если путь соответствует паттерну
     */
    static bool matches(const std::string& pattern, const std::string& path);

private:
    /**
     * @brief Разбить строку на сегменты по разделителю
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);
};