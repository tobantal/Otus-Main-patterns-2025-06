#pragma once
#include "domain/Rule.hpp"
#include "domain/PaginatedRules.hpp"
#include <optional>
#include <string>

/**
 * @file IRuleRepository.hpp
 * @brief Интерфейс репозитория для работы с правилами в БД
 * @author Anton Tobolkin
 */

/**
 * @interface IRuleRepository
 * @brief Порт для операций с правилами в базе данных
 */
class IRuleRepository
{
public:
    virtual ~IRuleRepository() = default;

    /**
     * @brief Создать новое правило
     */
    virtual bool create(const Rule& rule) = 0;

    /**
     * @brief Получить правило по shortId
     */
    virtual std::optional<Rule> findById(const std::string& shortId) = 0;

    /**
     * @brief Получить список всех правил с пагинацией
     */
    virtual PaginatedRules findAll(int page, int pageSize) = 0;

    /**
     * @brief Обновить правило
     */
    virtual bool update(const std::string& shortId, const Rule& rule) = 0;

    /**
     * @brief Удалить правило
     */
    virtual bool deleteById(const std::string& shortId) = 0;
};