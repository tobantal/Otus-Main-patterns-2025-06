#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/InvalidateCacheHandler.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"
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

class MockCacheInvalidator : public ICacheInvalidator {
public:
    MOCK_METHOD(bool, invalidate, (const std::string& shortId), (override));
    MOCK_METHOD(bool, invalidateAll, (), (override));
};

// --- Тесты ---

TEST(InvalidateCacheHandlerTest, Handle_InvalidateAllSuccessfully) {
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    EXPECT_CALL(req, getPath()).WillOnce(Return("/cache/invalidate"));
    EXPECT_CALL(*cacheInvalidator, invalidateAll()).WillOnce(Return(true));
    EXPECT_CALL(res, setStatus(200));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(_));

    InvalidateCacheHandler handler(cacheInvalidator);
    handler.handle(req, res);
}

TEST(InvalidateCacheHandlerTest, Handle_InvalidateByShortIdSuccessfully) {
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    EXPECT_CALL(req, getPath()).WillOnce(Return("/cache/invalidate/rule-123"));
    EXPECT_CALL(*cacheInvalidator, invalidate("rule-123")).WillOnce(Return(true));
    EXPECT_CALL(res, setStatus(200));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(_));

    InvalidateCacheHandler handler(cacheInvalidator);
    handler.handle(req, res);
}

TEST(InvalidateCacheHandlerTest, Handle_InvalidShortId) {
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    EXPECT_CALL(req, getPath()).WillOnce(Return("/cache/invalidate/"));
    EXPECT_CALL(res, setStatus(400));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Invalid shortId"})"));

    InvalidateCacheHandler handler(cacheInvalidator);
    handler.handle(req, res);
}

TEST(InvalidateCacheHandlerTest, Handle_FailInvalidateAll) {
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    EXPECT_CALL(req, getPath()).WillOnce(Return("/cache/invalidate"));
    EXPECT_CALL(*cacheInvalidator, invalidateAll()).WillOnce(Return(false));
    EXPECT_CALL(res, setStatus(500));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Failed to invalidate cache"})"));

    InvalidateCacheHandler handler(cacheInvalidator);
    handler.handle(req, res);
}

TEST(InvalidateCacheHandlerTest, Handle_FailInvalidateByShortId) {
    auto cacheInvalidator = std::make_shared<MockCacheInvalidator>();
    MockRequest req;
    MockResponse res;

    EXPECT_CALL(req, getPath()).WillOnce(Return("/cache/invalidate/rule-123"));
    EXPECT_CALL(*cacheInvalidator, invalidate("rule-123")).WillOnce(Return(false));
    EXPECT_CALL(res, setStatus(500));
    EXPECT_CALL(res, setHeader("Content-Type", "application/json"));
    EXPECT_CALL(res, setBody(R"({"error": "Failed to invalidate cache"})"));

    InvalidateCacheHandler handler(cacheInvalidator);
    handler.handle(req, res);
}
