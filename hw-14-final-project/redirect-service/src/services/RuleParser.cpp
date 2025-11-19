#include "services/RuleParser.hpp"
#include <stdexcept>
#include <sstream>
#include <cctype>

/**
 * @file RuleParser.cpp
 * @brief Реализация парсера DSL-выражений
 * @author Anton Tobolkin
 */

std::shared_ptr<ASTNode> RuleParser::parse(const std::string& condition)
{
    // Токенизация
    tokens_ = tokenize(condition);
    currentTokenIndex_ = 0;
    
    // Парсинг выражения
    auto ast = parseExpression();
    
    // Проверяем, что достигли конца
    if (!check(TokenType::End))
    {
        throw std::runtime_error("Unexpected token after expression: " + currentToken().value);
    }
    
    return ast;
}

std::vector<Token> RuleParser::tokenize(const std::string& condition)
{
    std::vector<Token> tokens;
    size_t i = 0;
    
    while (i < condition.length())
    {
        char ch = condition[i];
        
        // Пропускаем пробелы
        if (std::isspace(ch))
        {
            i++;
            continue;
        }
        
        // Литералы в кавычках: "string"
        if (ch == '"')
        {
            i++; // Пропускаем открывающую кавычку
            std::string literal;
            
            while (i < condition.length() && condition[i] != '"')
            {
                literal += condition[i];
                i++;
            }
            
            if (i >= condition.length())
            {
                throw std::runtime_error("Unclosed string literal");
            }
            
            i++; // Пропускаем закрывающую кавычку
            tokens.push_back({TokenType::Literal, literal});
            continue;
        }
        
        // Операторы
        if (ch == '=' && i + 1 < condition.length() && condition[i + 1] == '=')
        {
            tokens.push_back({TokenType::Operator, "=="});
            i += 2;
            continue;
        }
        
        if (ch == '!' && i + 1 < condition.length() && condition[i + 1] == '=')
        {
            tokens.push_back({TokenType::Operator, "!="});
            i += 2;
            continue;
        }
        
        if (ch == '<' && i + 1 < condition.length() && condition[i + 1] == '=')
        {
            tokens.push_back({TokenType::Operator, "<="});
            i += 2;
            continue;
        }
        
        if (ch == '>' && i + 1 < condition.length() && condition[i + 1] == '=')
        {
            tokens.push_back({TokenType::Operator, ">="});
            i += 2;
            continue;
        }
        
        if (ch == '<')
        {
            tokens.push_back({TokenType::Operator, "<"});
            i++;
            continue;
        }
        
        if (ch == '>')
        {
            tokens.push_back({TokenType::Operator, ">"});
            i++;
            continue;
        }
        
        // Скобки
        if (ch == '(')
        {
            tokens.push_back({TokenType::LeftParen, "("});
            i++;
            continue;
        }
        
        if (ch == ')')
        {
            tokens.push_back({TokenType::RightParen, ")"});
            i++;
            continue;
        }
        
        // Идентификаторы (переменные, AND, OR)
        if (std::isalpha(ch))
        {
            std::string identifier;
            
            while (i < condition.length() && (std::isalnum(condition[i]) || condition[i] == '_'))
            {
                identifier += condition[i];
                i++;
            }
            
            // Проверяем ключевые слова
            if (identifier == "AND")
            {
                tokens.push_back({TokenType::And, "AND"});
            }
            else if (identifier == "OR")
            {
                tokens.push_back({TokenType::Or, "OR"});
            }
            else
            {
                // Это переменная (browser, date, country)
                tokens.push_back({TokenType::Variable, identifier});
            }
            
            continue;
        }
        
        // Неизвестный символ
        throw std::runtime_error(std::string("Unexpected character: ") + ch);
    }
    
    // Добавляем токен конца
    tokens.push_back({TokenType::End, ""});
    
    return tokens;
}

const Token& RuleParser::currentToken() const
{
    if (currentTokenIndex_ >= tokens_.size())
    {
        throw std::runtime_error("Unexpected end of expression");
    }
    return tokens_[currentTokenIndex_];
}

void RuleParser::advance()
{
    if (currentTokenIndex_ < tokens_.size())
    {
        currentTokenIndex_++;
    }
}

bool RuleParser::check(TokenType type) const
{
    return currentToken().type == type;
}

void RuleParser::expect(TokenType type, const std::string& message)
{
    if (!check(type))
    {
        throw std::runtime_error(message);
    }
    advance();
}

std::shared_ptr<ASTNode> RuleParser::parseExpression()
{
    // expression := term (OR term)*
    auto left = parseTerm();
    
    while (check(TokenType::Or))
    {
        advance(); // Съедаем OR
        auto right = parseTerm();
        left = ASTNode::makeBinaryOp(OperatorType::Or, left, right);
    }
    
    return left;
}

std::shared_ptr<ASTNode> RuleParser::parseTerm()
{
    // term := factor (AND factor)*
    auto left = parseFactor();
    
    while (check(TokenType::And))
    {
        advance(); // Съедаем AND
        auto right = parseFactor();
        left = ASTNode::makeBinaryOp(OperatorType::And, left, right);
    }
    
    return left;
}

std::shared_ptr<ASTNode> RuleParser::parseFactor()
{
    // factor := comparison | '(' expression ')'
    
    // Обрабатываем скобки
    if (check(TokenType::LeftParen))
    {
        advance(); // Съедаем '('
        auto expr = parseExpression();
        expect(TokenType::RightParen, "Expected ')' after expression");
        return expr;
    }
    
    // Иначе это сравнение
    return parseComparison();
}

std::shared_ptr<ASTNode> RuleParser::parseComparison()
{
    // comparison := variable operator literal
    
    // Читаем переменную
    if (!check(TokenType::Variable))
    {
        throw std::runtime_error("Expected variable in comparison");
    }
    
    std::string variableName = currentToken().value;
    advance();
    
    // Читаем оператор
    if (!check(TokenType::Operator))
    {
        throw std::runtime_error("Expected operator after variable");
    }
    
    std::string opStr = currentToken().value;
    OperatorType op = parseOperator(opStr);
    advance();
    
    // Читаем литерал
    if (!check(TokenType::Literal))
    {
        throw std::runtime_error("Expected literal after operator");
    }
    
    std::string literalValue = currentToken().value;
    advance();
    
    // Строим AST
    auto left = ASTNode::makeVariable(variableName);
    auto right = ASTNode::makeLiteral(literalValue);
    
    return ASTNode::makeBinaryOp(op, left, right);
}

OperatorType RuleParser::parseOperator(const std::string& op)
{
    if (op == "==") return OperatorType::Equal;
    if (op == "!=") return OperatorType::NotEqual;
    if (op == "<") return OperatorType::Less;
    if (op == ">") return OperatorType::Greater;
    if (op == "<=") return OperatorType::LessOrEqual;
    if (op == ">=") return OperatorType::GreaterOrEqual;
    
    throw std::runtime_error("Unknown operator: " + op);
}