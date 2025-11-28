#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/DeleteRuleHandler.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"
#include "ports/IRuleService.hpp"
#include "ports/ICacheInvalidator.hpp"

using ::testing::_;
using ::testing::Return;

// Мок IRequest
class MockRequest : public IRequest
{
public:
    MOCK_METHOD(std::string, getBody, (), (const, override));
    MOCK_METHOD(std::string, getPath, (), (const, override));
    MOCK_METHOD(std::string, getMethod, (), (const, override));
    MOCK_METHOD((std::map<std::string, std::string>), getParams, (), (const, override));
    MOCK_METHOD((std::map<std::string, std::string>), getHeaders, (), (const, override));
    MOCK_METHOD(std::string, getIp, (), (const, override));
    MOCK_METHOD(int, getPort, (), (const, override));
};

// Мок IResponse
class MockResponse : public IResponse
{
public:
    MOCK_METHOD(void, setStatus, (int), (override));
    MOCK_METHOD(void, setBody, (const std::string &), (override));
    MOCK_METHOD(void, setHeader, (const std::string &, const std::string &), (override));
};

// Мок для IRuleService
class MockRuleService : public IRuleService {
public:
    MOCK_METHOD(bool, create, (const Rule& rule), (override));
    MOCK_METHOD(std::optional<Rule>, findById, (const std::string& shortId), (override));
    MOCK_METHOD(PaginatedRules, findAll, (int page, int pageSize), (override));
    MOCK_METHOD(bool, update, (const std::string& shortId, const Rule& rule), (override));
    MOCK_METHOD(bool, deleteById, (const std::string& shortId), (override));
};

// Мок для ICacheInvalidator
class MockCacheInvalidator : public ICacheInvalidator {
public:
    MOCK_METHOD(bool, invalidate, (const std::string& shortId), (override));
    MOCK_METHOD(bool, invalidateAll, (), (override));
};

// Тест успешного удаления
TEST(DeleteRuleHandlerTest, Handle_DeletesRuleSuccessfully) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    // Путь запроса с shortId
    std::string path = "/rules/rule-123";

    EXPECT_CALL(req, getPath())
        .WillOnce(Return(path));

    EXPECT_CALL(*ruleService, deleteById("rule-123"))
        .WillOnce(Return(true));

    EXPECT_CALL(*cacheInvalidator, invalidate("rule-123"))
        .WillOnce(Return(true));

    EXPECT_CALL(res, setStatus(204));
    EXPECT_CALL(res, setBody(""));

    DeleteRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}

// Тест удаления несуществующего правила
TEST(DeleteRuleHandlerTest, Handle_RuleNotFound) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-999";

    EXPECT_CALL(req, getPath())
        .WillOnce(Return(path));

    EXPECT_CALL(*ruleService, deleteById("rule-999"))
        .WillOnce(Return(false));

    EXPECT_CALL(res, setStatus(404));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Rule not found"})"));

    DeleteRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}

// Тест некорректного shortId
TEST(DeleteRuleHandlerTest, Handle_InvalidShortId) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/wrongpath/rule-123";

    EXPECT_CALL(req, getPath())
        .WillOnce(Return(path));

    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Invalid shortId"})"));

    DeleteRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}
