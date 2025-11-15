#pragma once

#include "BoostBeastApplication.hpp"
#include "ServerConfig.hpp"
#include "IHttpHandler.hpp"
#include <boost/di.hpp>
#include <memory>
#include <map>
#include <string>

namespace di = boost::di;

class RedirectServiceApp : public BoostBeastApplication
{
public:
    RedirectServiceApp(const ServerConfig& config);
    ~RedirectServiceApp() override;

protected:
    void configureInjection() override;
    void configureRoutes() override;
    void handleRequest(
        const boost::beast::http::request<boost::beast::http::string_body>& req,
        boost::beast::http::response<boost::beast::http::string_body>& res) override;

private:
    std::map<std::string, std::shared_ptr<IHttpHandler>> handlers_;
};