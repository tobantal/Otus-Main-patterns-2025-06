#pragma once
#include <string>

/**
 * @file RuleEntity.hpp
 * @brief сущность для хранения данных о правиле редиректа
 * @author Anton Tobolkin
 */

/**
 * @struct RuleEntity
 * @brief сущность правила редиректа
 */
struct RuleEntity
{
    std::string shortId;      ///< Короткий идентификатор (PRIMARY KEY)
    std::string targetUrl;    ///< Целевой URL для редиректа
    std::string condition;    ///< DSL-условие для активации правила
    std::string createdAt;    ///< Дата создания (ISO 8601)
    std::string updatedAt;    ///< Дата последнего обновления (ISO 8601)
};