#pragma once

#include "ASTNode.hpp"
#include <string>
#include <memory>
#include <vector>

/**
 * @file RuleParser.hpp
 * @brief Парсер DSL-выражений в AST
 * @author Anton Tobolkin
 */

/**
 * @enum TokenType
 * @brief Тип токена
 */
enum class TokenType {
    Variable,      ///< browser, date, country
    Literal,       ///< "string"
    Operator,      ///< ==, !=, <, >, <=, >=
    And,           ///< AND
    Or,            ///< OR
    LeftParen,     ///< (
    RightParen,    ///< )
    End            ///< Конец строки
};

/**
 * @struct Token
 * @brief Токен для парсинга
 */
struct Token {
    TokenType type;
    std::string value;
};

/**
 * @class RuleParser
 * @brief Парсер DSL-условий в AST
 * 
 * Поддерживаемая грамматика:
 * ```
 * expression := term (OR term)*
 * term := factor (AND factor)*
 * factor := comparison | '(' expression ')'
 * comparison := variable operator literal
 * variable := browser | date | country
 * operator := == | != | < | > | <= | >=
 * literal := "string"
 * ```
 * 
 * Примеры:
 * ```cpp
 * RuleParser parser;
 * auto ast = parser.parse("browser == \"chrome\"");
 * auto ast2 = parser.parse("(country == \"RU\") OR (country == \"US\")");
 * ```
 */
class RuleParser {
public:
    /**
     * @brief Распарсить DSL-выражение в AST
     * @param condition DSL-строка
     * @return Корневой узел AST
     * @throws std::runtime_error при синтаксической ошибке
     */
    std::shared_ptr<ASTNode> parse(const std::string& condition);

private:
    std::vector<Token> tokens_;
    size_t currentTokenIndex_;
    
    /**
     * @brief Разбить строку на токены
     */
    std::vector<Token> tokenize(const std::string& condition);
    
    /**
     * @brief Получить текущий токен
     */
    const Token& currentToken() const;
    
    /**
     * @brief Перейти к следующему токену
     */
    void advance();
    
    /**
     * @brief Проверить тип текущего токена
     */
    bool check(TokenType type) const;
    
    /**
     * @brief Съесть токен указанного типа или выбросить ошибку
     */
    void expect(TokenType type, const std::string& message);
    
    // Рекурсивный спуск (Recursive Descent Parsing)
    
    /**
     * @brief Разобрать выражение: term (OR term)*
     */
    std::shared_ptr<ASTNode> parseExpression();
    
    /**
     * @brief Разобрать терм: factor (AND factor)*
     */
    std::shared_ptr<ASTNode> parseTerm();
    
    /**
     * @brief Разобрать фактор: comparison | '(' expression ')'
     */
    std::shared_ptr<ASTNode> parseFactor();
    
    /**
     * @brief Разобрать сравнение: variable operator literal
     */
    std::shared_ptr<ASTNode> parseComparison();
    
    /**
     * @brief Преобразовать строку оператора в OperatorType
     */
    OperatorType parseOperator(const std::string& op);
};