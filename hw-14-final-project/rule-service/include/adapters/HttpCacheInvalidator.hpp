#pragma once

#include <memory>
#include <string>
#include "ports/ICacheInvalidator.hpp"
#include "settings/ICacheInvalidatorSettings.hpp"
#include "HttpClient.hpp"

/**
 * @brief HTTP-клиент для инвалидации кэша через IHttpClient
 */
class HttpCacheInvalidator : public ICacheInvalidator
{
private:
    std::shared_ptr<IHttpClient> httpClient_;
    std::string redirectServiceUrl_;

public:
    HttpCacheInvalidator(std::shared_ptr<IHttpClient> httpClient,
                         std::shared_ptr<ICacheInvalidatorSettings> settings);

    bool invalidate(const std::string& shortId) override;
    bool invalidateAll() override;
};
