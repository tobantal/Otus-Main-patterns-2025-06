#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "services/RuleService.hpp"
#include "ports/IRuleRepository.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::Invoke;

class MockRuleRepository : public IRuleRepository {
public:
    MOCK_METHOD(bool, create, (const Rule& rule), (override));
    MOCK_METHOD(std::optional<Rule>, findById, (const std::string& shortId), (override));
    MOCK_METHOD(PaginatedRules, findAll, (int page, int pageSize), (override));
    MOCK_METHOD(bool, update, (const std::string& shortId, const Rule& rule), (override));
    MOCK_METHOD(bool, deleteById, (const std::string& shortId), (override));
};

TEST(RuleServiceTest, Create_DelegatesToRepository)
{
    auto repo = std::make_shared<MockRuleRepository>();
    RuleService service(repo);

    Rule rule{"promo", "https://ya.ru", "cond"};

    EXPECT_CALL(*repo, create(rule))
        .WillOnce(Return(true));

    bool result = service.create(rule);

    EXPECT_TRUE(result);
}

TEST(RuleServiceTest, FindById_DelegatesToRepository)
{
    auto repo = std::make_shared<MockRuleRepository>();
    RuleService service(repo);

    Rule expected{"promo", "https://target", "cond"};

    EXPECT_CALL(*repo, findById("promo"))
        .WillOnce(Return(expected));

    auto result = service.findById("promo");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->shortId, "promo");
    EXPECT_EQ(result->targetUrl, "https://target");
    EXPECT_EQ(result->condition, "cond");
}

TEST(RuleServiceTest, FindAll_DelegatesToRepository)
{
    auto repo = std::make_shared<MockRuleRepository>();
    RuleService service(repo);

    PaginatedRules paginated;
    paginated.page = 2;
    paginated.pageSize = 5;
    paginated.totalCount = 100;
    paginated.rules = {
        {"id1", "u1", "c1"},
        {"id2", "u2", "c2"}
    };

    EXPECT_CALL(*repo, findAll(2, 5))
        .WillOnce(Return(paginated));

    auto result = service.findAll(2, 5);

    EXPECT_EQ(result.page, 2);
    EXPECT_EQ(result.pageSize, 5);
    EXPECT_EQ(result.totalCount, 100);
    ASSERT_EQ(result.rules.size(), 2);
}

TEST(RuleServiceTest, Update_DelegatesToRepository)
{
    auto repo = std::make_shared<MockRuleRepository>();
    RuleService service(repo);

    Rule rule{"promo", "url", "cond"};

    EXPECT_CALL(*repo, update("promo", rule))
        .WillOnce(Return(true));

    bool ok = service.update("promo", rule);

    EXPECT_TRUE(ok);
}

TEST(RuleServiceTest, DeleteById_DelegatesToRepository)
{
    auto repo = std::make_shared<MockRuleRepository>();
    RuleService service(repo);

    EXPECT_CALL(*repo, deleteById("promo"))
        .WillOnce(Return(true));

    bool ok = service.deleteById("promo");

    EXPECT_TRUE(ok);
}
