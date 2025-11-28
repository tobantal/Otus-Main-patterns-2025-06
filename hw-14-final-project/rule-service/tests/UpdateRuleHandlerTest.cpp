#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/UpdateRuleHandler.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"
#include "ports/IRuleService.hpp"
#include "ports/ICacheInvalidator.hpp"

using ::testing::_;
using ::testing::Return;

// --- Моки ---

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

class MockResponse : public IResponse
{
public:
    MOCK_METHOD(void, setStatus, (int), (override));
    MOCK_METHOD(void, setBody, (const std::string &), (override));
    MOCK_METHOD(void, setHeader, (const std::string &, const std::string &), (override));
};

class MockRuleService : public IRuleService {
public:
    MOCK_METHOD(bool, create, (const Rule& rule), (override));
    MOCK_METHOD(std::optional<Rule>, findById, (const std::string& shortId), (override));
    MOCK_METHOD(PaginatedRules, findAll, (int page, int pageSize), (override));
    MOCK_METHOD(bool, update, (const std::string& shortId, const Rule& rule), (override));
    MOCK_METHOD(bool, deleteById, (const std::string& shortId), (override));
};

class MockCacheInvalidator : public ICacheInvalidator {
public:
    MOCK_METHOD(bool, invalidate, (const std::string& shortId), (override));
    MOCK_METHOD(bool, invalidateAll, (), (override));
};

// --- Тесты ---

TEST(UpdateRuleHandlerTest, Handle_UpdatesRuleSuccessfully) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-123";
    std::string jsonBody = R"({"targetUrl": "https://example.com", "condition": "cond"})";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(req, getBody()).WillOnce(Return(jsonBody));
    EXPECT_CALL(*ruleService, update("rule-123", _)).WillOnce(Return(true));
    EXPECT_CALL(*cacheInvalidator, invalidate("rule-123")).WillOnce(Return(true));
    EXPECT_CALL(res, setStatus(200));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(_));

    UpdateRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}

TEST(UpdateRuleHandlerTest, Handle_RuleNotFound) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-999";
    std::string jsonBody = R"({"targetUrl": "https://example.com"})";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(req, getBody()).WillOnce(Return(jsonBody));
    EXPECT_CALL(*ruleService, update("rule-999", _)).WillOnce(Return(false));
    EXPECT_CALL(res, setStatus(404));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Rule not found"})"));

    UpdateRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}

TEST(UpdateRuleHandlerTest, Handle_InvalidShortId) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/wrongpath/rule-123";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Invalid shortId"})"));

    UpdateRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}

TEST(UpdateRuleHandlerTest, Handle_MissingTargetUrl) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-123";
    std::string jsonBody = R"({"condition": "cond"})";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(req, getBody()).WillOnce(Return(jsonBody));
    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Missing required field: targetUrl"})"));

    UpdateRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}

TEST(UpdateRuleHandlerTest, Handle_InvalidJSON) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-123";
    std::string jsonBody = "invalid json";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(req, getBody()).WillOnce(Return(jsonBody));
    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Invalid JSON"})"));

    UpdateRuleHandler handler(ruleService, cacheInvalidator);
    handler.handle(req, res);
}
