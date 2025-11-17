#include "adapters/AlwaysTrueDSLEvaluator.hpp"
#include <iostream>

/**
 * @file AlwaysTrueDSLEvaluator.cpp
 * @brief Реализация заглушки DSL интерпретатора
 * @author Anton Tobolkin
 */

bool AlwaysTrueDSLEvaluator::evaluate(const std::string& condition, const RedirectRequest& req)
{
    (void)condition;
    (void)req;
    
    std::cout << "[AlwaysTrueDSLEvaluator] Condition always evaluates to true" << std::endl;
    
    return true;
}