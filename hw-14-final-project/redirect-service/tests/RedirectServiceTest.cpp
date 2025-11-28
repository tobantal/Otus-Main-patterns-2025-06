#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "services/RedirectService.hpp"
#include "ports/IRuleClient.hpp"
#include "ports/IRuleEvaluator.hpp"
#include "domain/Rule.hpp"
#include "domain/RedirectRequest.hpp"
#include "domain/RedirectResult.hpp"

/**
 * @file RedirectServiceTest.cpp
 * @brief Unit-тесты для RedirectService с моками
 * @author Anton Tobolkin
 */

using ::testing::Return;
using ::testing::_;

// Mock для IRuleClient
class MockRuleClient : public IRuleClient
{
public:
    MOCK_METHOD(std::optional<Rule>, findByKey, (const std::string& key), (override));
};

// Mock для IRuleEvaluator
class MockRuleEvaluator : public IRuleEvaluator
{
public:
    MOCK_METHOD(bool, evaluate, (const std::string& condition, const RedirectRequest& request), (override));
};

// Fixture для тестов RedirectService
class RedirectServiceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mockRuleClient = std::make_shared<MockRuleClient>();
        mockEvaluator = std::make_shared<MockRuleEvaluator>();
        service = std::make_unique<RedirectService>(mockRuleClient, mockEvaluator);
    }

    std::shared_ptr<MockRuleClient> mockRuleClient;
    std::shared_ptr<MockRuleEvaluator> mockEvaluator;
    std::unique_ptr<RedirectService> service;
};

// Тест: правило найдено, условие выполнено → success
TEST_F(RedirectServiceTest, SuccessfulRedirect)
{
    // Arrange
    RedirectRequest request{"promo", "127.0.0.1", {{"User-Agent", "Chrome/120.0"}}};
    
    Rule rule{
        "promo",
        "https://example.com/promo",
        "browser == \"chrome\""
    };
    
    EXPECT_CALL(*mockRuleClient, findByKey("promo"))
        .WillOnce(Return(rule));
    
    EXPECT_CALL(*mockEvaluator, evaluate("browser == \"chrome\"", _))
        .WillOnce(Return(true));
    
    // Act
    RedirectResult result = service->redirect(request);
    
    // Assert
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.targetUrl, "https://example.com/promo");
    EXPECT_TRUE(result.errorMessage.empty());
}

// Тест: правило не найдено → error
TEST_F(RedirectServiceTest, RuleNotFound)
{
    // Arrange
    RedirectRequest request{"unknown", "127.0.0.1", {}};
    
    EXPECT_CALL(*mockRuleClient, findByKey("unknown"))
        .WillOnce(Return(std::nullopt));
    
    // Act
    RedirectResult result = service->redirect(request);
    
    // Assert
    EXPECT_FALSE(result.success);
    EXPECT_TRUE(result.targetUrl.empty());
    EXPECT_EQ(result.errorMessage, "Rule not found for key: unknown");
}

// Тест: условие DSL не выполнено → error
TEST_F(RedirectServiceTest, ConditionNotSatisfied)
{
    // Arrange
    RedirectRequest request{"promo", "127.0.0.1", {{"User-Agent", "Firefox/100.0"}}};
    
    Rule rule{
        "promo",
        "https://example.com/promo",
        "browser == \"chrome\""
    };
    
    EXPECT_CALL(*mockRuleClient, findByKey("promo"))
        .WillOnce(Return(rule));
    
    EXPECT_CALL(*mockEvaluator, evaluate("browser == \"chrome\"", _))
        .WillOnce(Return(false));
    
    // Act
    RedirectResult result = service->redirect(request);
    
    // Assert
    EXPECT_FALSE(result.success);
    EXPECT_TRUE(result.targetUrl.empty());
    EXPECT_EQ(result.errorMessage, "Condition not satisfied");
}

// Тест: пустое условие → evaluate вызывается и возвращает true
TEST_F(RedirectServiceTest, EmptyCondition)
{
    // Arrange
    RedirectRequest request{"blog", "127.0.0.1", {}};
    
    Rule rule{
        "blog",
        "https://blog.example.com",
        ""  // пустое условие
    };
    
    EXPECT_CALL(*mockRuleClient, findByKey("blog"))
        .WillOnce(Return(rule));
    
    // RedirectService всё равно вызывает evaluate для пустого условия
    EXPECT_CALL(*mockEvaluator, evaluate("", _))
        .WillOnce(Return(true));
    
    // Act
    RedirectResult result = service->redirect(request);
    
    // Assert
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.targetUrl, "https://blog.example.com");
    EXPECT_TRUE(result.errorMessage.empty());
}

// Тест: правило с пробелами в условии
TEST_F(RedirectServiceTest, ConditionWithWhitespace)
{
    // Arrange
    RedirectRequest request{"docs", "192.168.1.1", {}};
    
    Rule rule{
        "docs",
        "https://docs.example.com",
        "   "  // только пробелы
    };
    
    EXPECT_CALL(*mockRuleClient, findByKey("docs"))
        .WillOnce(Return(rule));
    
    // RedirectService вызывает evaluate даже для whitespace
    EXPECT_CALL(*mockEvaluator, evaluate("   ", _))
        .WillOnce(Return(true));
    
    // Act
    RedirectResult result = service->redirect(request);
    
    // Assert
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.targetUrl, "https://docs.example.com");
}

// Тест: несколько последовательных вызовов с разными результатами
TEST_F(RedirectServiceTest, MultipleCallsWithDifferentResults)
{
    // Arrange
    RedirectRequest request1{"promo", "127.0.0.1", {{"User-Agent", "Chrome/120.0"}}};
    RedirectRequest request2{"blog", "127.0.0.1", {}};
    
    Rule rule1{"promo", "https://example.com/promo", "browser == \"chrome\""};
    Rule rule2{"blog", "https://blog.example.com", ""};
    
    EXPECT_CALL(*mockRuleClient, findByKey("promo"))
        .WillOnce(Return(rule1));
    
    EXPECT_CALL(*mockRuleClient, findByKey("blog"))
        .WillOnce(Return(rule2));
    
    EXPECT_CALL(*mockEvaluator, evaluate("browser == \"chrome\"", _))
        .WillOnce(Return(true));
    
    EXPECT_CALL(*mockEvaluator, evaluate("", _))
        .WillOnce(Return(true));
    
    // Act
    RedirectResult result1 = service->redirect(request1);
    RedirectResult result2 = service->redirect(request2);
    
    // Assert
    EXPECT_TRUE(result1.success);
    EXPECT_EQ(result1.targetUrl, "https://example.com/promo");
    
    EXPECT_TRUE(result2.success);
    EXPECT_EQ(result2.targetUrl, "https://blog.example.com");
}

// Тест: правило с сложным DSL-условием
TEST_F(RedirectServiceTest, ComplexDSLCondition)
{
    // Arrange
    RedirectRequest request{"special", "127.0.0.1", {{"User-Agent", "Chrome/120.0"}}};
    
    Rule rule{
        "special",
        "https://special.example.com",
        "(browser == \"chrome\" OR browser == \"firefox\") AND date < \"2030-01-01\""
    };
    
    EXPECT_CALL(*mockRuleClient, findByKey("special"))
        .WillOnce(Return(rule));
    
    EXPECT_CALL(*mockEvaluator, evaluate(
        "(browser == \"chrome\" OR browser == \"firefox\") AND date < \"2030-01-01\"", _))
        .WillOnce(Return(true));
    
    // Act
    RedirectResult result = service->redirect(request);
    
    // Assert
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.targetUrl, "https://special.example.com");
}

// Тест: правило с разными IP-адресами
TEST_F(RedirectServiceTest, DifferentClientIPs)
{
    // Arrange
    RedirectRequest request1{"test", "192.168.1.1", {}};
    RedirectRequest request2{"test", "10.0.0.5", {}};
    
    Rule rule{"test", "https://test.example.com", ""};
    
    EXPECT_CALL(*mockRuleClient, findByKey("test"))
        .Times(2)
        .WillRepeatedly(Return(rule));
    
    EXPECT_CALL(*mockEvaluator, evaluate("", _))
        .Times(2)
        .WillRepeatedly(Return(true));
    
    // Act
    RedirectResult result1 = service->redirect(request1);
    RedirectResult result2 = service->redirect(request2);
    
    // Assert
    EXPECT_TRUE(result1.success);
    EXPECT_TRUE(result2.success);
    EXPECT_EQ(result1.targetUrl, "https://test.example.com");
    EXPECT_EQ(result2.targetUrl, "https://test.example.com");
}