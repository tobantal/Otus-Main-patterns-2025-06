#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/ListRulesHandler.hpp"
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

TEST(ListRulesHandlerTest, Handle_ReturnsPaginatedRulesSuccessfully) {
    auto ruleService = std::make_shared<MockRuleService>();
    MockRequest req;
    MockResponse res;

    std::map<std::string, std::string> params = {{"page", "2"}, {"size", "5"}};

    PaginatedRules result;
    result.page = 2;
    result.pageSize = 5;
    result.totalCount = 12;
    result.rules = {
        {"rule-6", "https://url6.com", "cond6"},
        {"rule-7", "https://url7.com", "cond7"}
    };

    EXPECT_CALL(req, getParams()).WillOnce(Return(params));
    EXPECT_CALL(*ruleService, findAll(2, 5)).WillOnce(Return(result));
    EXPECT_CALL(res, setStatus(200));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(_));

    ListRulesHandler handler(ruleService);
    handler.handle(req, res);
}

TEST(ListRulesHandlerTest, Handle_UsesDefaultPaginationWhenParamsMissing) {
    auto ruleService = std::make_shared<MockRuleService>();
    MockRequest req;
    MockResponse res;

    std::map<std::string, std::string> params;  // пустой map

    PaginatedRules result;
    result.page = 1;
    result.pageSize = 10;
    result.totalCount = 0;
    result.rules = {};

    EXPECT_CALL(req, getParams()).WillOnce(Return(params));
    EXPECT_CALL(*ruleService, findAll(1, 10)).WillOnce(Return(result));
    EXPECT_CALL(res, setStatus(200));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(_));

    ListRulesHandler handler(ruleService);
    handler.handle(req, res);
}

TEST(ListRulesHandlerTest, Handle_InvalidPaginationParameters) {
    auto ruleService = std::make_shared<MockRuleService>();
    MockRequest req;
    MockResponse res;

    std::map<std::string, std::string> params = {{"page", "0"}, {"size", "101"}};

    EXPECT_CALL(req, getParams()).WillOnce(Return(params));
    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Invalid pagination parameters"})"));

    ListRulesHandler handler(ruleService);
    handler.handle(req, res);
}
