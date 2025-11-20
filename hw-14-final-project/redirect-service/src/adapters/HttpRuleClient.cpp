#include "adapters/HttpRuleClient.hpp"
#include "SimpleRequest.hpp"
#include "SimpleResponse.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>

using json = nlohmann::json;

std::pair<std::string, int> parseUrl(const std::string &url)
{
    std::regex urlRegex(R"(http://([^:]+):(\d+))");
    std::smatch match;

    if (std::regex_match(url, match, urlRegex))
    {
        std::string host = match[1].str();
        int port = std::stoi(match[2].str());
        return {host, port};
    }

    throw std::runtime_error("Invalid URL format: " + url);
}

HttpRuleClient::HttpRuleClient(std::shared_ptr<IHttpClient> httpClient,
                               std::shared_ptr<IRuleServiceSettings> settings)
    : httpClient_(httpClient), settings_(settings)
{
}

std::optional<Rule> HttpRuleClient::findByKey(const std::string &key)
{
    try
    {
        std::cout << "[HttpRuleClient] Fetching rule by key: " << key << std::endl;

        auto [host, port] = parseUrl(settings_->getUrl());

        SimpleRequest request(
            "GET",
            "/rules/" + key,
            "",
            host,
            port,
            {{"Accept", "application/json"}});

        SimpleResponse response(200, "");

        if (!httpClient_->send(request, response))
        {
            std::cerr << "[HttpRuleClient] Failed to send request" << std::endl;
            return std::nullopt;
        }

        if (response.getStatus() != 200)
        {
            std::cerr << "[HttpRuleClient] Rule not found, status: " << response.getStatus() << std::endl;
            return std::nullopt;
        }

        json data = json::parse(response.getBody());

        Rule rule{
            data["shortId"].get<std::string>(),
            data["targetUrl"].get<std::string>(),
            data["condition"].get<std::string>()};

        std::cout << "[HttpRuleClient] Rule found: " << rule.key << std::endl;
        return rule;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[HttpRuleClient] Error: " << e.what() << std::endl;
        return std::nullopt;
    }
}
