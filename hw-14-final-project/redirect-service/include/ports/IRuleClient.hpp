#pragma once

#include "domain/Rule.hpp"
#include <optional>
#include <string>

/**
 * @file IRuleClient.hpp
 * @brief Интерфейс порта для хранилища правил
 * @author Anton Tobolkin
 */

/**
 * @class IRuleClient
 * @brief Порт для доступа к хранилищу правил переадресации
 */
class IRuleClient
{
public:
    virtual ~IRuleClient() = default;

    /**
     * @brief Найти правило по ключу
     * @param key Короткий ID правила
     * @return Правило если найдено
     */
    virtual std::optional<Rule> findByKey(const std::string& key) = 0;
};