#include <gtest/gtest.h>
#include "adapters/HttpRuleClient.hpp"
#include "SimpleRequest.hpp"
#include "SimpleResponse.hpp"
#include "Environment.hpp"
#include <optional>
#include <map>
#include <memory>
#include "settings/RuleServiceSettings.hpp"

/**
 * Заглушка IHttpClient
 */
class DummyHttpClient : public IHttpClient
{
public:
    bool send(const IRequest &req, IResponse &res) override
    {
        // Симулируем возврат ответа с JSON
        if (req.getPath() == "/rules/testKey")
        {
            res.setStatus(200);
            res.setBody(R"({"shortId":"testKey","targetUrl":"http://target","condition":"browser==\"chrome\""})");
        }
        else
        {
            res.setStatus(404);
        }
        return true;
    }
};

/**
 * Заглушка IRulesCache
 */
class DummyRulesCache : public IRulesCache
{
    std::map<std::string, Rule> store_;

public:
    std::optional<Rule> find(const std::string &key) override
    {
        auto it = store_.find(key);
        if (it != store_.end())
            return it->second;
        return std::nullopt;
    }

    void put(const std::string &key, const Rule &rule) override
    {
        store_[key] = rule;
    }
    void remove(const std::string &id) override {}
    void clear() override {}
};

TEST(HttpRuleClientTest, FindByKeyReturnsRule)
{
    auto env = std::make_shared<Environment>();
    env->setProperty("services.rule_service_url", std::string("http://localhost:8080"));

    auto settings = std::make_shared<RuleServiceSettings>(env);
    auto httpClient = std::make_shared<DummyHttpClient>();
    auto cache = std::make_shared<DummyRulesCache>();

    HttpRuleClient client(httpClient, settings, cache);

    // Первый вызов, правило не в кэше
    auto ruleOpt = client.findByKey("testKey");
    ASSERT_TRUE(ruleOpt.has_value());
    EXPECT_EQ(ruleOpt->key, "testKey");
    EXPECT_EQ(ruleOpt->targetUrl, "http://target");
    EXPECT_EQ(ruleOpt->condition, "browser==\"chrome\"");

    // Второй вызов, правило должно взять из кэша
    auto cachedRuleOpt = client.findByKey("testKey");
    ASSERT_TRUE(cachedRuleOpt.has_value());
    EXPECT_EQ(cachedRuleOpt->key, "testKey");
}

TEST(HttpRuleClientTest, FindByKeyNotFound)
{
    auto env = std::make_shared<Environment>();
    env->setProperty("services.rule_service_url", std::string("http://localhost:8080"));

    auto settings = std::make_shared<RuleServiceSettings>(env);
    auto httpClient = std::make_shared<DummyHttpClient>();
    auto cache = std::make_shared<DummyRulesCache>();

    HttpRuleClient client(httpClient, settings, cache);

    auto ruleOpt = client.findByKey("missingKey");
    EXPECT_FALSE(ruleOpt.has_value());
}
