#include "services/ASTNode.hpp"

/**
 * @file ASTNode.cpp
 * @brief Реализация фабричных методов для создания узлов AST
 * @author Anton Tobolkin
 */

std::shared_ptr<ASTNode> ASTNode::makeLiteral(const std::string& value)
{
    auto node = std::make_shared<ASTNode>();
    node->type = NodeType::Literal;
    node->value = value;
    return node;
}

std::shared_ptr<ASTNode> ASTNode::makeVariable(const std::string& name)
{
    auto node = std::make_shared<ASTNode>();
    node->type = NodeType::Variable;
    node->value = name;
    return node;
}

std::shared_ptr<ASTNode> ASTNode::makeBinaryOp(
    OperatorType op,
    std::shared_ptr<ASTNode> left,
    std::shared_ptr<ASTNode> right)
{
    auto node = std::make_shared<ASTNode>();
    node->type = NodeType::BinaryOp;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}