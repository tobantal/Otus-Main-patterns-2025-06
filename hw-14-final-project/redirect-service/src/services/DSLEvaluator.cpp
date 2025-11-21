#include "services/DSLEvaluator.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * @file DSLEvaluator.cpp
 * @brief Реализация DSL-интерпретатора с кэшированием
 * @author Anton Tobolkin
 */

DSLEvaluator::DSLEvaluator()
{
    std::cout << "[DSLEvaluator] Created" << std::endl;
}

bool DSLEvaluator::evaluate(const std::string &condition, const RedirectRequest &req)
{
    // Проверяем кэш
    auto it = cache_.find(condition);
    if (it != cache_.end())
    {
        return evaluateAST(it->second, req);
    }

    try
    {
        auto ast = parser_.parse(condition);
        cache_[condition] = ast;
        std::cout << "[DSLEvaluator] Parsed and cached condition: " << condition << std::endl;
        return evaluateAST(ast, req);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[DSLEvaluator] Parse error for condition '" << condition << "': " << e.what() << std::endl;
        return false;
    }
}

bool DSLEvaluator::evaluateAST(const std::shared_ptr<ASTNode> &ast, const RedirectRequest &req)
{
    if (!ast)
        return false;

    switch (ast->type)
    {
    case NodeType::Literal:
        return true;

    case NodeType::Variable:
        return true;

    case NodeType::BinaryOp:
    {
        if (ast->op == OperatorType::And)
            return evaluateAST(ast->left, req) && evaluateAST(ast->right, req);

        if (ast->op == OperatorType::Or)
            return evaluateAST(ast->left, req) || evaluateAST(ast->right, req);

        if (!ast->left || ast->left->type != NodeType::Variable)
        {
            std::cerr << "[DSLEvaluator] Left operand is not a variable" << std::endl;
            return false;
        }

        if (!ast->right || ast->right->type != NodeType::Literal)
        {
            std::cerr << "[DSLEvaluator] Right operand is not a literal" << std::endl;
            return false;
        }

        std::string leftValue = getVariableValue(ast->left->value, req);
        std::string rightValue = ast->right->value;

        // Если переменная неизвестна, возвращаем false
        if (leftValue.empty() && ast->left->value != "")
        {
            return false;
        }

        return compareValues(leftValue, rightValue, ast->op);
    }
    }

    return false;
}

std::string DSLEvaluator::getVariableValue(
    const std::string &varName,
    const RedirectRequest &req)
{
    //
    // --- browser ---
    //
    if (varName == "browser")
    {
        auto it = req.headers.find("User-Agent");
        std::string ua = (it != req.headers.end()) ? it->second : "";
        std::string uaLower = ua;
        std::transform(uaLower.begin(), uaLower.end(), uaLower.begin(), ::tolower);

        // порядок важен!

        // Microsoft Edge (Chromium)
        if (uaLower.find("edg") != std::string::npos)
            return "edge";

        // Firefox
        if (uaLower.find("firefox") != std::string::npos)
            return "firefox";

        // Chrome (но не Edge)
        if (uaLower.find("chrome") != std::string::npos &&
            uaLower.find("edg") == std::string::npos)
        {
            return "chrome";
        }

        // Safari (но не Chrome)
        if (uaLower.find("safari") != std::string::npos &&
            uaLower.find("chrome") == std::string::npos)
        {
            return "safari";
        }

        return "unknown";
    }

    //
    // --- ip ---
    //
    if (varName == "ip")
    {
        return req.ip;
    }

    //
    // --- date ---
    //
    if (varName == "date")
    {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }

    //
    // --- country (заглушка) ---
    //
    if (varName == "country")
    {
        return "RU";
    }

    //
    // --- header.<NAME> (универсальный механизм) ---
    //
    if (varName.rfind("header.", 0) == 0)
    {
        const std::string headerName = varName.substr(7);
        auto it = req.headers.find(headerName);
        if (it != req.headers.end())
            return it->second;
        return "";
    }

    //
    // --- неизвестная переменная ---
    //
    std::cerr << "[DSLEvaluator] Unknown variable: " << varName << std::endl;
    return "";
}

bool DSLEvaluator::compareValues(const std::string &left, const std::string &right, OperatorType op)
{
    switch (op)
    {
    case OperatorType::Equal:
        return left == right;

    case OperatorType::NotEqual:
        return left != right;

    case OperatorType::Less:
        return left < right;

    case OperatorType::Greater:
        return left > right;

    case OperatorType::LessOrEqual:
        return left <= right;

    case OperatorType::GreaterOrEqual:
        return left >= right;

    default:
        std::cerr << "[DSLEvaluator] Unknown operator" << std::endl;
        return false;
    }
}
