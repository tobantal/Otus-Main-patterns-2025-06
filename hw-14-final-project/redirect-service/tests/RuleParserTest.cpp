#include <gtest/gtest.h>
#include "services/RuleParser.hpp"

/**
 * @file RuleParserTest.cpp
 * @brief Unit-тесты для RuleParser
 * @author Anton Tobolkin
 */

// Тест: простое сравнение
TEST(RuleParserTest, SimpleComparison)
{
    RuleParser parser;
    
    auto ast = parser.parse("browser == \"chrome\"");
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::Equal);
    
    ASSERT_NE(ast->left, nullptr);
    EXPECT_EQ(ast->left->type, NodeType::Variable);
    EXPECT_EQ(ast->left->value, "browser");
    
    ASSERT_NE(ast->right, nullptr);
    EXPECT_EQ(ast->right->type, NodeType::Literal);
    EXPECT_EQ(ast->right->value, "chrome");
}

// Тест: операторы сравнения
TEST(RuleParserTest, ComparisonOperators)
{
    RuleParser parser;
    
    auto ast1 = parser.parse("date < \"2026-01-01\"");
    EXPECT_EQ(ast1->op, OperatorType::Less);
    
    auto ast2 = parser.parse("date > \"2025-01-01\"");
    EXPECT_EQ(ast2->op, OperatorType::Greater);
    
    auto ast3 = parser.parse("date <= \"2026-01-01\"");
    EXPECT_EQ(ast3->op, OperatorType::LessOrEqual);
    
    auto ast4 = parser.parse("date >= \"2025-01-01\"");
    EXPECT_EQ(ast4->op, OperatorType::GreaterOrEqual);
    
    auto ast5 = parser.parse("country != \"US\"");
    EXPECT_EQ(ast5->op, OperatorType::NotEqual);
}

// Тест: AND оператор
TEST(RuleParserTest, AndOperator)
{
    RuleParser parser;
    
    auto ast = parser.parse("browser == \"chrome\" AND date < \"2026-01-01\"");
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::And);
    
    // Левая часть: browser == "chrome"
    ASSERT_NE(ast->left, nullptr);
    EXPECT_EQ(ast->left->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->left->op, OperatorType::Equal);
    
    // Правая часть: date < "2026-01-01"
    ASSERT_NE(ast->right, nullptr);
    EXPECT_EQ(ast->right->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->right->op, OperatorType::Less);
}

// Тест: OR оператор
TEST(RuleParserTest, OrOperator)
{
    RuleParser parser;
    
    auto ast = parser.parse("country == \"RU\" OR country == \"US\"");
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::Or);
    
    ASSERT_NE(ast->left, nullptr);
    EXPECT_EQ(ast->left->op, OperatorType::Equal);
    
    ASSERT_NE(ast->right, nullptr);
    EXPECT_EQ(ast->right->op, OperatorType::Equal);
}

// Тест: приоритет AND выше OR
TEST(RuleParserTest, OperatorPrecedence)
{
    RuleParser parser;
    
    // browser == "chrome" AND date < "2026" OR country == "RU"
    // Должно быть: (browser == "chrome" AND date < "2026") OR country == "RU"
    auto ast = parser.parse("browser == \"chrome\" AND date < \"2026\" OR country == \"RU\"");
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::Or);  // Корень - OR
    
    // Левая часть - это AND
    ASSERT_NE(ast->left, nullptr);
    EXPECT_EQ(ast->left->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->left->op, OperatorType::And);
    
    // Правая часть - это сравнение
    ASSERT_NE(ast->right, nullptr);
    EXPECT_EQ(ast->right->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->right->op, OperatorType::Equal);
}

// Тест: скобки
TEST(RuleParserTest, Parentheses)
{
    RuleParser parser;
    
    auto ast = parser.parse("(country == \"RU\" OR country == \"US\") AND browser == \"chrome\"");
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::And);  // Корень - AND
    
    // Левая часть - это OR (из-за скобок)
    ASSERT_NE(ast->left, nullptr);
    EXPECT_EQ(ast->left->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->left->op, OperatorType::Or);
}

// Тест: вложенные скобки
TEST(RuleParserTest, NestedParentheses)
{
    RuleParser parser;
    
    auto ast = parser.parse("((browser == \"chrome\"))");
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::Equal);
}

// Тест: пробелы игнорируются
TEST(RuleParserTest, WhitespaceHandling)
{
    RuleParser parser;
    
    auto ast1 = parser.parse("browser==\"chrome\"");
    auto ast2 = parser.parse("  browser  ==  \"chrome\"  ");
    
    ASSERT_NE(ast1, nullptr);
    ASSERT_NE(ast2, nullptr);
    
    EXPECT_EQ(ast1->left->value, "browser");
    EXPECT_EQ(ast2->left->value, "browser");
}

// Тест: ошибка - незакрытая кавычка
TEST(RuleParserTest, ErrorUnclosedQuote)
{
    RuleParser parser;
    
    EXPECT_THROW(parser.parse("browser == \"chrome"), std::runtime_error);
}

// Тест: ошибка - незакрытая скобка
TEST(RuleParserTest, ErrorUnclosedParenthesis)
{
    RuleParser parser;
    
    EXPECT_THROW(parser.parse("(browser == \"chrome\""), std::runtime_error);
}

// Тест: ошибка - неизвестный оператор
TEST(RuleParserTest, ErrorInvalidOperator)
{
    RuleParser parser;
    
    EXPECT_THROW(parser.parse("browser = \"chrome\""), std::runtime_error);
}

// Тест: ошибка - отсутствует литерал
TEST(RuleParserTest, ErrorMissingLiteral)
{
    RuleParser parser;
    
    EXPECT_THROW(parser.parse("browser =="), std::runtime_error);
}

// Тест: ошибка - отсутствует переменная
TEST(RuleParserTest, ErrorMissingVariable)
{
    RuleParser parser;
    
    EXPECT_THROW(parser.parse("== \"chrome\""), std::runtime_error);
}

// Тест: сложное выражение
TEST(RuleParserTest, ComplexExpression)
{
    RuleParser parser;
    
    auto ast = parser.parse(
        "(browser == \"chrome\" OR browser == \"firefox\") AND "
        "(country == \"RU\" OR country == \"US\") AND "
        "date < \"2026-01-01\""
    );
    
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->type, NodeType::BinaryOp);
    EXPECT_EQ(ast->op, OperatorType::And);
}