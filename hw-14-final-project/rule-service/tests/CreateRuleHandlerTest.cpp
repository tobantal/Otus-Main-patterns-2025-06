#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/CreateRuleHandler.hpp"
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

class MockCacheInvalidator : public ICacheInvalidator {
public:
    MOCK_METHOD(bool, invalidate, (const std::string& shortId), (override));
    MOCK_METHOD(bool, invalidateAll, (), (override));
};

// Тест для CreateRuleHandler
TEST(CreateRuleHandlerTest, Handle_CreatesRuleSuccessfully) {
    auto ruleService = std::make_shared<MockRuleService>();
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    // Корректный JSON в теле запроса
    std::string jsonBody = R"({
        "shortId": "rule-123",
        "targetUrl": "http://example.com",
        "condition": "some condition"
    })";

    EXPECT_CALL(req, getBody())
        .WillOnce(Return(jsonBody));

    EXPECT_CALL(*ruleService, create(_))
        .WillOnce(Return(true));

    EXPECT_CALL(*cacheInvalidator, invalidate("rule-123"))
        .WillOnce(Return(true));

    CreateRuleHandler handler(ruleService, cacheInvalidator);

    handler.handle(req, res);
}


