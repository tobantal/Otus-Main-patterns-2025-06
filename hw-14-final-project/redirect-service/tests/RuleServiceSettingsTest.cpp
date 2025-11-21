#include <gtest/gtest.h>
#include "settings/RuleServiceSettings.hpp"
#include "Environment.hpp"

// Тест: корректное чтение URL
TEST(RuleServiceSettingsTest, ReturnsUrlWhenPresent)
{
    auto env = std::make_shared<Environment>();
    env->setProperty("services.rule_service_url", std::string("http://localhost:8080"));

    RuleServiceSettings settings(env);

    EXPECT_EQ(settings.getUrl(), "http://localhost:8080");
}

// Тест: выброс исключения при отсутствии URL
TEST(RuleServiceSettingsTest, ThrowsWhenUrlMissing)
{
    auto env = std::make_shared<Environment>();

    EXPECT_THROW({
        RuleServiceSettings settings(env);
    }, std::runtime_error);
}
