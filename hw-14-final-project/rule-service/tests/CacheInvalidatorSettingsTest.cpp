#include <gtest/gtest.h>

#include "settings/CacheInvalidatorSettings.hpp"
#include "Environment.hpp"

// ===== Тест 1: корректная загрузка redirect_service.url =====

TEST(CacheInvalidatorSettingsTest, LoadsRedirectServiceUrl) {
    // given
    auto env = std::make_shared<Environment>();
    env->setProperty("redirect_service.url", std::string("http://localhost:9000"));

    // when
    CacheInvalidatorSettings settings(env);

    // then
    EXPECT_EQ(settings.getRedirectServiceUrl(), "http://localhost:9000");
}

// ===== Тест 2: отсутствие redirect_service.url должно вызвать exception =====

TEST(CacheInvalidatorSettingsTest, ThrowsIfMissingSetting) {
    auto env = std::make_shared<Environment>();

    // expect
    EXPECT_THROW(
        {
            try {
                CacheInvalidatorSettings settings(env);
            } catch (const std::runtime_error& err) {
                EXPECT_STREQ(err.what(), "Missing required setting: redirect_service.url");
                throw;
            }
        },
        std::runtime_error
    );
}
