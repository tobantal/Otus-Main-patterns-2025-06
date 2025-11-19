#pragma once

#include <string>
#include <memory>

/**
 * @file ASTNode.hpp
 * @brief AST (Abstract Syntax Tree) для DSL-выражений
 * @author Anton Tobolkin
 */

/**
 * @enum NodeType
 * @brief Тип узла AST
 */
enum class NodeType {
    Literal,      ///< Литерал: "chrome", "2026-01-01"
    Variable,     ///< Переменная: browser, date, country
    BinaryOp      ///< Бинарная операция: ==, !=, <, >, AND, OR
};

/**
 * @enum OperatorType
 * @brief Тип оператора для бинарных операций
 */
enum class OperatorType {
    Equal,           ///< ==
    NotEqual,        ///< !=
    Less,            ///< 
    Greater,         ///< >
    LessOrEqual,     ///< <=
    GreaterOrEqual,  ///< >=
    And,             ///< AND
    Or               ///< OR
};

/**
 * @struct ASTNode
 * @brief Узел дерева абстрактного синтаксиса
 * 
 * Примеры:
 * - Literal: {type=Literal, value="chrome"}
 * - Variable: {type=Variable, value="browser"}
 * - BinaryOp: {type=BinaryOp, op=Equal, left=Variable("browser"), right=Literal("chrome")}
 */
struct ASTNode {
    NodeType type;                        ///< Тип узла
    std::string value;                    ///< Значение (для Literal и Variable)
    OperatorType op;                      ///< Оператор (для BinaryOp)
    std::shared_ptr<ASTNode> left;        ///< Левый потомок (для BinaryOp)
    std::shared_ptr<ASTNode> right;       ///< Правый потомок (для BinaryOp)
    
    /**
     * @brief Создать узел-литерал
     */
    static std::shared_ptr<ASTNode> makeLiteral(const std::string& value);
    
    /**
     * @brief Создать узел-переменную
     */
    static std::shared_ptr<ASTNode> makeVariable(const std::string& name);
    
    /**
     * @brief Создать узел бинарной операции
     */
    static std::shared_ptr<ASTNode> makeBinaryOp(
        OperatorType op,
        std::shared_ptr<ASTNode> left,
        std::shared_ptr<ASTNode> right
    );
};