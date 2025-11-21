#include "adapters/InMemoryRuleRepository.hpp"
#include <gtest/gtest.h>

class InMemoryRuleRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        repo = std::make_unique<InMemoryRuleRepository>();
    }

    void TearDown() override
    {
        repo.reset();
    }

    std::unique_ptr<InMemoryRuleRepository> repo;
};

TEST_F(InMemoryRuleRepositoryTest, InitializationTest)
{
    // Проверяем, что репозиторий создал все тестовые правила
    auto page1 = repo->findAll(1, 100);
    EXPECT_EQ(page1.totalCount, 13);

    // Проверяем наличие конкретного правила
    auto ruleOpt = repo->findById("promo");
    ASSERT_TRUE(ruleOpt.has_value());
    EXPECT_EQ(ruleOpt->targetUrl, "https://example.com/promo");
    EXPECT_EQ(ruleOpt->condition, "country == \"RU\"");
}

TEST_F(InMemoryRuleRepositoryTest, CreateDuplicateRule)
{
    Rule duplicate{"promo", "https://example.com/promo2", "country == \"US\""};
    bool created = repo->create(duplicate);
    EXPECT_FALSE(created);  // Дубликат не создаётся
}

TEST_F(InMemoryRuleRepositoryTest, FindByIdNonExistent)
{
    auto ruleOpt = repo->findById("nonexistent");
    EXPECT_FALSE(ruleOpt.has_value());
}

TEST_F(InMemoryRuleRepositoryTest, UpdateRule)
{
    Rule updated{"promo", "https://example.com/promo-updated", "country == \"RU\""};
    bool updatedOk = repo->update("promo", updated);
    EXPECT_TRUE(updatedOk);

    auto ruleOpt = repo->findById("promo");
    ASSERT_TRUE(ruleOpt.has_value());
    EXPECT_EQ(ruleOpt->targetUrl, "https://example.com/promo-updated");
}

TEST_F(InMemoryRuleRepositoryTest, UpdateNonExistentRule)
{
    Rule updated{"fake", "https://fake.com", "country == \"US\""};
    bool updatedOk = repo->update("fake", updated);
    EXPECT_FALSE(updatedOk);
}

TEST_F(InMemoryRuleRepositoryTest, DeleteRule)
{
    bool deleted = repo->deleteById("promo");
    EXPECT_TRUE(deleted);

    auto ruleOpt = repo->findById("promo");
    EXPECT_FALSE(ruleOpt.has_value());
}

TEST_F(InMemoryRuleRepositoryTest, DeleteNonExistentRule)
{
    bool deleted = repo->deleteById("nonexistent");
    EXPECT_FALSE(deleted);
}

TEST_F(InMemoryRuleRepositoryTest, PaginationTest)
{
    // Получаем первые 5 правил
    auto page1 = repo->findAll(1, 5);
    EXPECT_EQ(page1.rules.size(), 5);
    EXPECT_EQ(page1.totalCount, 13);

    // Следующая страница
    auto page2 = repo->findAll(2, 5);
    EXPECT_EQ(page2.rules.size(), 5);

    // Последняя страница (13 правил => 3-я страница 3 элемента)
    auto page3 = repo->findAll(3, 5);
    EXPECT_EQ(page3.rules.size(), 3);
}
