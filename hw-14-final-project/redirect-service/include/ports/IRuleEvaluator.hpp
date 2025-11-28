#pragma once

#include "domain/RedirectRequest.hpp"
#include <string>

/**
 * @file IRuleEvaluator.hpp
 * @brief Интерфейс порта для оценки DSL условий
 * @author Anton Tobolkin
 */

/**
 * @class IRuleEvaluator
 * @brief Порт для интерпретации DSL условий правил
 */
class IRuleEvaluator
{
public:
    virtual ~IRuleEvaluator() = default;

    /**
     * @brief Оценить DSL условие
     * @param condition DSL строка (например "browser == chrome")
     * @param req Контекст запроса
     * @return true если условие выполнено
     */
    virtual bool evaluate(const std::string& condition, const RedirectRequest& req) = 0;
};