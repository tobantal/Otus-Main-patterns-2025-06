#pragma once

#include "IWebApplication.hpp"
#include "IEnvironment.hpp"
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

/**
 * @file BoostBeastApplication.hpp
 * @brief Базовый класс приложения на Boost.Beast
 * @author Anton Tobolkin
 */
class BoostBeastApplication : public IWebApplication
{
public:
    BoostBeastApplication();
    virtual ~BoostBeastApplication();

    void loadEnvironment(int argc, char* argv[]) override;
    void start() override;
    void stop();

    std::shared_ptr<IEnvironment> getEnvironment() const
    {
        return env_;
    }

protected:
    void handleSession(boost::asio::ip::tcp::socket socket);

    virtual void handleRequest(
        const boost::beast::http::request<boost::beast::http::string_body>& req,
        boost::beast::http::response<boost::beast::http::string_body>& res,
        const std::string& clientIp) = 0;
    
    void loadJsonToEnvironment(const nlohmann::json& j, const std::string& prefix = "");

protected:
    std::shared_ptr<IEnvironment> env_;
    std::unique_ptr<boost::asio::io_context> ioContext_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    bool running_;
};