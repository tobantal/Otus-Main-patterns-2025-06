#pragma once

#include "ports/IRuleEvaluator.hpp"
#include "services/ASTNode.hpp"
#include "services/RuleParser.hpp"
#include <memory>
#include <unordered_map>

/**
 * @file DSLEvaluator.hpp
 * @brief DSL-интерпретатор с кэшированием распарсенных условий
 * @author Anton Tobolkin
 */

/**
 * @class DSLEvaluator
 * @brief Вычисляет DSL-условия с внутренним кэшем AST
 * 
 * Кэширует распарсенные AST для ускорения повторных вычислений.
 * Поддерживаемый синтаксис:
 * - Переменные: browser, date, country
 * - Операторы: ==, !=, <, >, <=, >=, AND, OR
 * - Литералы: "строка"
 * - Скобки: (выражение)
 */
class DSLEvaluator : public IRuleEvaluator
{
public:
    DSLEvaluator();
    
    /**
     * @brief Вычислить DSL-условие
     * @param condition DSL-строка
     * @param req Контекст запроса
     * @return true если условие выполнено
     */
    bool evaluate(const std::string& condition, const RedirectRequest& req) override;

private:
    // Кэш: condition → AST
    std::unordered_map<std::string, std::shared_ptr<ASTNode>> cache_;
    
    // Парсер
    RuleParser parser_;
    
    /**
     * @brief Вычислить AST-узел
     */
    bool evaluateAST(const std::shared_ptr<ASTNode>& ast, const RedirectRequest& req);
    
    /**
     * @brief Получить значение переменной из запроса
     */
    std::string getVariableValue(const std::string& varName, const RedirectRequest& req);
    
    /**
     * @brief Сравнить два значения
     */
    bool compareValues(const std::string& left, const std::string& right, OperatorType op);
};