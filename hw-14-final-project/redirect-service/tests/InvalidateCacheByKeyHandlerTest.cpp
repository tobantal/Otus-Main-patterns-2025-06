#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "handlers/InvalidateCacheByKeyHandler.hpp"
#include "SimpleRequest.hpp"
#include "SimpleResponse.hpp"
#include "cache/IRulesCache.hpp"

using ::testing::_;

// Мок кеша
class MockRulesCache : public IRulesCache {
public:
    MOCK_METHOD(std::optional<Rule>, find, (const std::string& id), (override));
    MOCK_METHOD(void, clear, (), (override));
    MOCK_METHOD(void, put, (const std::string& id, const Rule& rule), (override));
    MOCK_METHOD(void, remove, (const std::string& id), (override));
};

TEST(InvalidateCacheByKeyHandlerTest, RemovesRuleFromCache) {
    auto cache = std::make_shared<MockRulesCache>();
    InvalidateCacheByKeyHandler handler(cache);

    // создаём реальные объекты IRequest/IResponse
    SimpleRequest request("DELETE", "/cache/invalidate/test-rule-id", "", "127.0.0.1", 8080, {});
    SimpleResponse response;

    // проверяем, что remove вызван с нужным ключом
    EXPECT_CALL(*cache, remove("test-rule-id")).Times(1);

    // вызываем хэндлер
    handler.handle(request, response);

    // проверяем, что статус 204 и тело пустое
    EXPECT_EQ(response.getStatus(), 204);
    EXPECT_EQ(response.getBody(), "");
}
