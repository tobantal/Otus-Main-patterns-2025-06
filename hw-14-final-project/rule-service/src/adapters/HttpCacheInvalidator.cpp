#include "adapters/HttpCacheInvalidator.hpp"
#include "SimpleRequest.hpp"
#include "SimpleResponse.hpp"
#include <iostream>
#include <regex>

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

HttpCacheInvalidator::HttpCacheInvalidator(std::shared_ptr<IHttpClient> httpClient,
                                           std::shared_ptr<ICacheInvalidatorSettings> settings)
    : httpClient_(httpClient), redirectServiceUrl_(settings->getRedirectServiceUrl())
{
    std::cout << "[HttpCacheInvalidator] Created with redirect-service URL: " 
              << redirectServiceUrl_ << std::endl;
}

bool HttpCacheInvalidator::invalidate(const std::string& shortId)
{
    try
    {
        std::cout << "[HttpCacheInvalidator] Invalidating cache for: " << shortId << std::endl;

        auto [host, port] = parseUrl(redirectServiceUrl_);

        SimpleRequest request(
            "DELETE",
            "/cache/invalidate/" + shortId,
            "",
            host,
            port,
            {});

        SimpleResponse response(204, "");

        if (!httpClient_->send(request, response))
        {
            std::cerr << "[HttpCacheInvalidator] Failed to send request" << std::endl;
            return false;
        }

        bool success = (response.getStatus() == 204);
        
        if (success)
        {
            std::cout << "[HttpCacheInvalidator] Cache invalidation successful" << std::endl;
        }
        else
        {
            std::cerr << "[HttpCacheInvalidator] Cache invalidation failed with status: " 
                      << response.getStatus() << std::endl;
        }

        return success;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[HttpCacheInvalidator] Error: " << e.what() << std::endl;
        return false;
    }
}

bool HttpCacheInvalidator::invalidateAll()
{
    try
    {
        std::cout << "[HttpCacheInvalidator] Invalidating all cache" << std::endl;

        auto [host, port] = parseUrl(redirectServiceUrl_);

        SimpleRequest request(
            "DELETE",
            "/cache/invalidate",
            "",
            host,
            port,
            {});

        SimpleResponse response(204, "");

        if (!httpClient_->send(request, response))
        {
            std::cerr << "[HttpCacheInvalidator] Failed to send request" << std::endl;
            return false;
        }

        bool success = (response.getStatus() == 204);
        
        if (success)
        {
            std::cout << "[HttpCacheInvalidator] Cache invalidation successful" << std::endl;
        }
        else
        {
            std::cerr << "[HttpCacheInvalidator] Cache invalidation failed with status: " 
                      << response.getStatus() << std::endl;
        }

        return success;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[HttpCacheInvalidator] Error: " << e.what() << std::endl;
        return false;
    }
}
