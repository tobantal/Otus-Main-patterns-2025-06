#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "adapters/HttpCacheInvalidator.hpp"
#include "settings/CacheInvalidatorSettings.hpp"
#include "Environment.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// ===== МОК-КЛИЕНТ =====

class MockHttpClient : public IHttpClient {
public:
    MOCK_METHOD(bool, send, (const IRequest& request, IResponse& response), (override));
};


// ===== ФАБРИКА окружения =====

std::shared_ptr<ICacheInvalidatorSettings> makeSettings(const std::string& url) {
    auto env = std::make_shared<Environment>();
    env->setProperty("redirect_service.url", url);
    return std::make_shared<CacheInvalidatorSettings>(env);
}

// ===================================================================
// === ТЕСТ 1: invalidate(shortId) — успешный запрос (204) ===========
// ===================================================================

TEST(HttpCacheInvalidatorTest, Invalidate_Success) {
    auto http = std::make_shared<MockHttpClient>();
    auto settings = makeSettings("http://localhost:9000");

    HttpCacheInvalidator inv(http, settings);

    EXPECT_CALL(*http, send(_, _))
        .WillOnce(Invoke([](const IRequest& req, IResponse& res) {
            EXPECT_EQ(req.getMethod(), "DELETE");
            EXPECT_EQ(req.getPath(), "/cache/invalidate/promo");
            EXPECT_EQ(req.getPort(), 9000);

            res.setStatus(204);
            return true;
        }));

    EXPECT_TRUE(inv.invalidate("promo"));
}

// ===================================================================
// === ТЕСТ 2: invalidate(shortId) — send() вернул false =============
// ===================================================================

TEST(HttpCacheInvalidatorTest, Invalidate_SendFails) {
    auto http = std::make_shared<MockHttpClient>();
    auto settings = makeSettings("http://localhost:8080");

    HttpCacheInvalidator inv(http, settings);

    EXPECT_CALL(*http, send(_, _)).WillOnce(Return(false));

    EXPECT_FALSE(inv.invalidate("any"));
}

// ===================================================================
// === ТЕСТ 3: invalidateAll() — успешный запрос (204) ===============
// ===================================================================

TEST(HttpCacheInvalidatorTest, InvalidateAll_Success) {
    auto http = std::make_shared<MockHttpClient>();
    auto settings = makeSettings("http://127.0.0.1:7000");

    HttpCacheInvalidator inv(http, settings);

    EXPECT_CALL(*http, send(_, _))
        .WillOnce(Invoke([](const IRequest& req, IResponse& res) {
            EXPECT_EQ(req.getMethod(), "DELETE");
            EXPECT_EQ(req.getPath(), "/cache/invalidate");
            EXPECT_EQ(req.getPort(), 7000);

            res.setStatus(204);
            return true;
        }));

    EXPECT_TRUE(inv.invalidateAll());
}

// ===================================================================
// === ТЕСТ 4: invalidateAll() — HTTP статус != 204 ==================
// ===================================================================

TEST(HttpCacheInvalidatorTest, InvalidateAll_FailsWithNon204) {
    auto http = std::make_shared<MockHttpClient>();
    auto settings = makeSettings("http://localhost:8000");

    HttpCacheInvalidator inv(http, settings);

    EXPECT_CALL(*http, send(_, _))
        .WillOnce(Invoke([](const IRequest& req, IResponse& res) {
            res.setStatus(500);
            return true;
        }));

    EXPECT_FALSE(inv.invalidateAll());
}

// ===================================================================
// === ТЕСТ 5: некорректный URL приводит к false =====================
// ===================================================================

TEST(HttpCacheInvalidatorTest, InvalidUrl_ReturnsFalse) {
    auto http = std::make_shared<MockHttpClient>();

    // URL без порта → parseUrl() должно упасть → invalidate() вернуть false
    auto settings = makeSettings("http://bad-url-value");

    HttpCacheInvalidator inv(http, settings);

    EXPECT_FALSE(inv.invalidate("promo"));
}
