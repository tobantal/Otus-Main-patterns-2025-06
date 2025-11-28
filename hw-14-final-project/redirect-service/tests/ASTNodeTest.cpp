#include <gtest/gtest.h>
#include "services/ASTNode.hpp"

TEST(ASTNodeTest, MakeLiteralNode) {
    auto node = ASTNode::makeLiteral("42");

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->type, NodeType::Literal);
    EXPECT_EQ(node->value, "42");
    EXPECT_EQ(node->left, nullptr);
    EXPECT_EQ(node->right, nullptr);
}

TEST(ASTNodeTest, MakeVariableNode) {
    auto node = ASTNode::makeVariable("x");

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->type, NodeType::Variable);
    EXPECT_EQ(node->value, "x");
    EXPECT_EQ(node->left, nullptr);
    EXPECT_EQ(node->right, nullptr);
}

TEST(ASTNodeTest, MakeBinaryOpNode) {
    auto left = ASTNode::makeLiteral("1");
    auto right = ASTNode::makeLiteral("2");
    auto node = ASTNode::makeBinaryOp(OperatorType::Equal, left, right);

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->type, NodeType::BinaryOp);
    EXPECT_EQ(node->op, OperatorType::Equal);
    EXPECT_EQ(node->left, left);
    EXPECT_EQ(node->right, right);
    EXPECT_EQ(node->value, "");  // у бинарного узла value пустое
}


TEST(ASTNodeTest, ComplexTreeStructure) {
    auto varNode = ASTNode::makeVariable("age");
    auto litNode = ASTNode::makeLiteral("18");
    auto binOpNode = ASTNode::makeBinaryOp(OperatorType::GreaterOrEqual, varNode, litNode);

    ASSERT_NE(binOpNode, nullptr);
    EXPECT_EQ(binOpNode->type, NodeType::BinaryOp);
    EXPECT_EQ(binOpNode->op, OperatorType::GreaterOrEqual);
    EXPECT_EQ(binOpNode->left, varNode);
    EXPECT_EQ(binOpNode->right, litNode);
}