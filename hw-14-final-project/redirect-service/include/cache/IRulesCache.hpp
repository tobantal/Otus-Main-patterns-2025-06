#pragma once

#include <string>
#include <memory>
#include <optional>

// Forward declaration
class Rule;

/**
 * @brief Интерфейс кэша правил
 */
class IRulesCache
{
public:
    virtual ~IRulesCache() = default;

    /**
     * @brief Найти правило по ID
     * @param id ID правила
     * @return Правило или std::nullopt если не найдено
     */
    virtual std::optional<Rule> find(const std::string& id) = 0;

    /**
     * @brief Удалить правило из кэша по ID
     * @param id ID правила
     */
    virtual void remove(const std::string& id) = 0;

    /**
     * @brief Очистить весь кэш
     */
    virtual void clear() = 0;

    /**
     * @brief Добавить правило в кэш
     * @param id ID правила
     * @param rule Само правило
     */
    virtual void put(const std::string& id, const Rule& rule) = 0;
};
