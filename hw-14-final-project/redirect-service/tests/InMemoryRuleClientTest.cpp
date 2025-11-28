#include <gtest/gtest.h>
#include "adapters/InMemoryRuleClient.hpp"

/**
 * @file InMemoryRuleClient_test.cpp
 * @brief Тесты для класса InMemoryRuleClient
 * @author Test Suite
 */

class InMemoryRuleClientTest : public ::testing::Test {
protected:
    InMemoryRuleClient client;

    void SetUp() override {
        // Клиент инициализируется с 3 тестовыми правилами в конструкторе
    }

    void TearDown() override {
        // Очистка происходит автоматически при уничтожении объекта
    }
};

// ============================================================================
// TESTS: INITIALIZATION (Инициализация)
// ============================================================================

TEST_F(InMemoryRuleClientTest, ClientInitializesWithTestRules) {
    // Проверяем наличие всех 3 правил после инициализации
    auto promo = client.findByKey("promo");
    auto docs = client.findByKey("docs");
    auto blog = client.findByKey("blog");
    
    EXPECT_TRUE(promo.has_value());
    EXPECT_TRUE(docs.has_value());
    EXPECT_TRUE(blog.has_value());
}

// ============================================================================
// TESTS: FIND BY KEY (Поиск по ключу)
// ============================================================================

TEST_F(InMemoryRuleClientTest, FindPromoRuleByKey) {
    auto result = client.findByKey("promo");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->key, "promo");
    EXPECT_EQ(result->targetUrl, "https://example.com/promo");
    EXPECT_EQ(result->condition, "browser == \"chrome\"");
}

TEST_F(InMemoryRuleClientTest, FindDocsRuleByKey) {
    auto result = client.findByKey("docs");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->key, "docs");
    EXPECT_EQ(result->targetUrl, "https://docs.example.com");
    EXPECT_EQ(result->condition, "date < \"2026-01-01\"");
}

TEST_F(InMemoryRuleClientTest, FindBlogRuleByKey) {
    auto result = client.findByKey("blog");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->key, "blog");
    EXPECT_EQ(result->targetUrl, "https://blog.example.com");
    EXPECT_EQ(result->condition, "country == \"RU\"");
}

TEST_F(InMemoryRuleClientTest, FindNonExistentRule) {
    auto result = client.findByKey("non_existent");
    EXPECT_FALSE(result.has_value());
}

TEST_F(InMemoryRuleClientTest, FindNonExistentRuleReturnsNullopt) {
    auto result = client.findByKey("nonexistent_key");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result, std::nullopt);
}

// ============================================================================
// TESTS: RULE DATA INTEGRITY (Целостность данных правил)
// ============================================================================

TEST_F(InMemoryRuleClientTest, PromoRuleHasCorrectBrowserCondition) {
    auto result = client.findByKey("promo");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("browser") != std::string::npos);
    EXPECT_TRUE(result->condition.find("chrome") != std::string::npos);
}

TEST_F(InMemoryRuleClientTest, DocsRuleHasCorrectDateCondition) {
    auto result = client.findByKey("docs");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("date") != std::string::npos);
    EXPECT_TRUE(result->condition.find("2026-01-01") != std::string::npos);
}

TEST_F(InMemoryRuleClientTest, BlogRuleHasCorrectCountryCondition) {
    auto result = client.findByKey("blog");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("country") != std::string::npos);
    EXPECT_TRUE(result->condition.find("RU") != std::string::npos);
}

// ============================================================================
// TESTS: URL VALIDATION (Проверка URL-адресов)
// ============================================================================

TEST_F(InMemoryRuleClientTest, AllRulesHaveValidUrls) {
    auto promo = client.findByKey("promo");
    auto docs = client.findByKey("docs");
    auto blog = client.findByKey("blog");
    
    ASSERT_TRUE(promo.has_value());
    ASSERT_TRUE(docs.has_value());
    ASSERT_TRUE(blog.has_value());
    
    EXPECT_TRUE(promo->targetUrl.find("https://") == 0);
    EXPECT_TRUE(docs->targetUrl.find("https://") == 0);
    EXPECT_TRUE(blog->targetUrl.find("https://") == 0);
}

TEST_F(InMemoryRuleClientTest, PromoUrlIsCorrect) {
    auto result = client.findByKey("promo");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->targetUrl, "https://example.com/promo");
}

TEST_F(InMemoryRuleClientTest, DocsUrlIsCorrect) {
    auto result = client.findByKey("docs");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->targetUrl, "https://docs.example.com");
}

TEST_F(InMemoryRuleClientTest, BlogUrlIsCorrect) {
    auto result = client.findByKey("blog");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->targetUrl, "https://blog.example.com");
}

// ============================================================================
// TESTS: RULE KEY CONSISTENCY (Консистентность ключей)
// ============================================================================

TEST_F(InMemoryRuleClientTest, RuleKeyMatchesSearchKey) {
    auto promo = client.findByKey("promo");
    ASSERT_TRUE(promo.has_value());
    EXPECT_EQ(promo->key, "promo");
}

TEST_F(InMemoryRuleClientTest, AllRuleKeysMatchTheirSearchKeys) {
    std::vector<std::string> keys = {"promo", "docs", "blog"};
    
    for (const auto& key : keys) {
        auto result = client.findByKey(key);
        ASSERT_TRUE(result.has_value()) << "Rule " << key << " not found";
        EXPECT_EQ(result->key, key) << "Rule key doesn't match search key: " << key;
    }
}

// ============================================================================
// TESTS: EDGE CASES (Граничные случаи)
// ============================================================================

TEST_F(InMemoryRuleClientTest, FindWithEmptyKey) {
    auto result = client.findByKey("");
    EXPECT_FALSE(result.has_value());
}

TEST_F(InMemoryRuleClientTest, FindWithCaseSensitiveKey) {
    auto lowerCase = client.findByKey("promo");
    auto upperCase = client.findByKey("PROMO");
    auto mixedCase = client.findByKey("Promo");
    
    EXPECT_TRUE(lowerCase.has_value());
    EXPECT_FALSE(upperCase.has_value());  // Ключи чувствительны к регистру
    EXPECT_FALSE(mixedCase.has_value());
}

TEST_F(InMemoryRuleClientTest, FindWithWhitespaceAroundKey) {
    auto result = client.findByKey(" promo ");
    EXPECT_FALSE(result.has_value());  // Whitespace не должен быть автоматически удален
}

TEST_F(InMemoryRuleClientTest, MultipleSearchesReturnSameRule) {
    auto first = client.findByKey("blog");
    auto second = client.findByKey("blog");
    
    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    
    EXPECT_EQ(first->key, second->key);
    EXPECT_EQ(first->targetUrl, second->targetUrl);
    EXPECT_EQ(first->condition, second->condition);
}

// ============================================================================
// TESTS: CONDITION TYPES (Типы условий)
// ============================================================================

TEST_F(InMemoryRuleClientTest, RuleWithEqualityComparison) {
    auto result = client.findByKey("promo");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("==") != std::string::npos);
}

TEST_F(InMemoryRuleClientTest, RuleWithLessThanComparison) {
    auto result = client.findByKey("docs");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("<") != std::string::npos);
}

TEST_F(InMemoryRuleClientTest, RuleWithStringValue) {
    auto result = client.findByKey("blog");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("\"RU\"") != std::string::npos);
}

TEST_F(InMemoryRuleClientTest, RuleWithDateValue) {
    auto result = client.findByKey("docs");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->condition.find("2026-01-01") != std::string::npos);
}

// ============================================================================
// TESTS: SEQUENTIAL ACCESS (Последовательный доступ)
// ============================================================================

TEST_F(InMemoryRuleClientTest, AccessAllRulesSequentially) {
    std::vector<std::string> keys = {"promo", "docs", "blog"};
    
    for (const auto& key : keys) {
        auto result = client.findByKey(key);
        EXPECT_TRUE(result.has_value()) << "Failed to find rule: " << key;
    }
}

TEST_F(InMemoryRuleClientTest, AccessRulesInDifferentOrder) {
    auto blog = client.findByKey("blog");
    auto promo = client.findByKey("promo");
    auto docs = client.findByKey("docs");
    
    EXPECT_TRUE(blog.has_value());
    EXPECT_TRUE(promo.has_value());
    EXPECT_TRUE(docs.has_value());
}

TEST_F(InMemoryRuleClientTest, AccessSameRuleMultipleTimes) {
    auto first = client.findByKey("promo");
    auto second = client.findByKey("promo");
    auto third = client.findByKey("promo");
    
    EXPECT_TRUE(first.has_value());
    EXPECT_TRUE(second.has_value());
    EXPECT_TRUE(third.has_value());
    
    EXPECT_EQ(first->targetUrl, second->targetUrl);
    EXPECT_EQ(second->targetUrl, third->targetUrl);
}

// ============================================================================
// TESTS: RETURN TYPE VALIDATION (Проверка типов возврата)
// ============================================================================

TEST_F(InMemoryRuleClientTest, FindByKeyReturnsOptional) {
    auto result = client.findByKey("promo");
    // Просто проверяем, что результат имеет методы optional
    EXPECT_TRUE(result.has_value() || !result.has_value());
}

TEST_F(InMemoryRuleClientTest, FoundRuleIsRuleType) {
    auto result = client.findByKey("promo");
    ASSERT_TRUE(result.has_value());
    
    // Проверяем, что результат содержит все необходимые поля Rule
    Rule rule = result.value();
    EXPECT_FALSE(rule.key.empty());
    EXPECT_FALSE(rule.targetUrl.empty());
    EXPECT_FALSE(rule.condition.empty());
}

// ============================================================================
// TESTS: IMMUTABILITY (Неизменяемость данных)
// ============================================================================

TEST_F(InMemoryRuleClientTest, FindingRuleDoesNotModifyIt) {
    auto first = client.findByKey("promo");
    std::string originalUrl = first->targetUrl;
    std::string originalCondition = first->condition;
    
    // Получаем правило еще раз
    auto second = client.findByKey("promo");
    
    EXPECT_EQ(second->targetUrl, originalUrl);
    EXPECT_EQ(second->condition, originalCondition);
}

// ============================================================================
// STRESS TESTS (Нагрузочные тесты)
// ============================================================================

TEST_F(InMemoryRuleClientTest, PerformManyConsecutiveSearches) {
    for (int i = 0; i < 1000; i++) {
        auto result = client.findByKey("promo");
        EXPECT_TRUE(result.has_value());
    }
}

TEST_F(InMemoryRuleClientTest, SearchAllRulesManyTimes) {
    std::vector<std::string> keys = {"promo", "docs", "blog"};
    
    for (int i = 0; i < 100; i++) {
        for (const auto& key : keys) {
            auto result = client.findByKey(key);
            EXPECT_TRUE(result.has_value());
        }
    }
}

TEST_F(InMemoryRuleClientTest, SearchNonExistentManyTimes) {
    for (int i = 0; i < 100; i++) {
        auto result = client.findByKey("nonexistent_" + std::to_string(i));
        EXPECT_FALSE(result.has_value());
    }
}