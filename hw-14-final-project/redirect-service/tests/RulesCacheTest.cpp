#include <gtest/gtest.h>
#include "cache/RulesCache.hpp"


/**
 * @file RulesCacheTest.cpp
 * @brief Unit-тесты для RulesCache
 */

TEST(RulesCacheTest, PutAndFindRule)
{
    RulesCache cache;
    Rule rule{"promo", "https://example.com", "browser == chrome"};

    cache.put("promo", rule);

    auto result = cache.find("promo");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->key, "promo");
    EXPECT_EQ(result->targetUrl, "https://example.com");
    EXPECT_EQ(result->condition, "browser == chrome");
}

TEST(RulesCacheTest, FindNonExistentRule)
{
    RulesCache cache;
    auto result = cache.find("nonexistent");
    EXPECT_FALSE(result.has_value());
}

TEST(RulesCacheTest, RemoveRule)
{
    RulesCache cache;
    Rule rule{"docs", "https://docs.example.com", ""};
    cache.put("docs", rule);

    EXPECT_TRUE(cache.find("docs").has_value());

    cache.remove("docs");

    EXPECT_FALSE(cache.find("docs").has_value());
}

TEST(RulesCacheTest, ClearCache)
{
    RulesCache cache;
    cache.put("a", Rule{"a", "url1", ""});
    cache.put("b", Rule{"b", "url2", ""});

    EXPECT_TRUE(cache.find("a").has_value());
    EXPECT_TRUE(cache.find("b").has_value());

    cache.clear();

    EXPECT_FALSE(cache.find("a").has_value());
    EXPECT_FALSE(cache.find("b").has_value());
}

TEST(RulesCacheTest, OverwriteExistingRule)
{
    RulesCache cache;
    cache.put("promo", Rule{"promo", "https://example.com", ""});
    cache.put("promo", Rule{"promo", "https://new.example.com", "browser == firefox"});

    auto result = cache.find("promo");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->targetUrl, "https://new.example.com");
    EXPECT_EQ(result->condition, "browser == firefox");
}
