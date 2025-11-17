#pragma once

#include "BoostBeastApplication.hpp"
#include "IHttpHandler.hpp"
#include <boost/di.hpp>
#include <memory>
#include <map>
#include <string>

namespace di = boost::di;

/**
 * @file RedirectServiceApp.hpp
 * @brief Главный класс приложения переадресации
 * @author Anton Tobolkin
 */

/**
 * @class RedirectServiceApp
 * @brief Приложение сервиса переадресации
 */
class RedirectServiceApp : public BoostBeastApplication
{
public:
    RedirectServiceApp();
    ~RedirectServiceApp() override;

protected:
    void loadEnvironment(int argc, char* argv[]) override;
    void configureInjection() override;
    void handleRequest(
        const boost::beast::http::request<boost::beast::http::string_body>& req,
        boost::beast::http::response<boost::beast::http::string_body>& res) override;

private:
    std::map<std::string, std::shared_ptr<IHttpHandler>> handlers_;
};