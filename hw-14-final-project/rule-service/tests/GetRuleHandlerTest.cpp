#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/GetRuleHandler.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"
#include "ports/IRuleService.hpp"

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

// --- Тесты ---

TEST(GetRuleHandlerTest, Handle_ReturnsRuleSuccessfully) {
    auto ruleService = std::make_shared<MockRuleService>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-123";
    Rule rule{"rule-123", "https://example.com", "cond"};

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(*ruleService, findById("rule-123")).WillOnce(Return(std::make_optional(rule)));
    EXPECT_CALL(res, setStatus(200));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(_));

    GetRuleHandler handler(ruleService);
    handler.handle(req, res);
}

TEST(GetRuleHandlerTest, Handle_RuleNotFound) {
    auto ruleService = std::make_shared<MockRuleService>();
    MockRequest req;
    MockResponse res;

    std::string path = "/rules/rule-999";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(*ruleService, findById("rule-999")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(res, setStatus(404));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Rule not found"})"));

    GetRuleHandler handler(ruleService);
    handler.handle(req, res);
}

TEST(GetRuleHandlerTest, Handle_InvalidShortId) {
    auto ruleService = std::make_shared<MockRuleService>();
    MockRequest req;
    MockResponse res;

    std::string path = "/wrongpath/rule-123";

    EXPECT_CALL(req, getPath()).WillOnce(Return(path));
    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Invalid shortId"})"));

    GetRuleHandler handler(ruleService);
    handler.handle(req, res);
}
