#pragma once
#include "Rule.hpp"
#include <vector>

/**
 * @file PaginatedRules.hpp
 * @brief Пагинированный список правил
 * @author Anton Tobolkin
 */

/**
 * @struct PaginatedRules
 * @brief Результат запроса списка правил с пагинацией
 */
struct PaginatedRules
{
    std::vector<Rule> rules;  ///< Список правил
    int totalCount;           ///< Общее количество правил в БД
    int page;                 ///< Текущая страница (начинается с 1)
    int pageSize;             ///< Размер страницы
};