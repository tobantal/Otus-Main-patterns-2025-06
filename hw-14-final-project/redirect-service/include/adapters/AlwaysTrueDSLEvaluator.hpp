#pragma once

#include "ports/IRuleEvaluator.hpp"

/**
 * @file AlwaysTrueDSLEvaluator.hpp
 * @brief Заглушка - всегда возвращает true
 * @author Anton Tobolkin
 */

/**
 * @class AlwaysTrueDSLEvaluator
 * @brief Заглушка для оценки DSL условий (всегда true)
 */
class AlwaysTrueDSLEvaluator : public IRuleEvaluator
{
public:
    bool evaluate(const std::string& condition, const RedirectRequest& req) override;
};