#pragma once
#include "IWebApplication.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

class IRequest;
class IResponse;

/**
 * @file BoostBeastApplication.hpp
 * @brief Базовый класс для HTTP-приложений на Boost.Beast
 * @author Anton Tobolkin
 */

class BoostBeastApplication : public IWebApplication
{
public:
    BoostBeastApplication();
    virtual ~BoostBeastApplication();

    void start() override;
    void stop();
    void loadEnvironment(int argc, char* argv[]) override;

protected:
    /**
     * @brief Обработать HTTP-запрос (работа с абстракциями)
     */
    virtual void handleRequest(IRequest& req, IResponse& res) = 0;

private:
    std::unique_ptr<boost::asio::io_context> ioContext_;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    bool running_;

    void handleSession(boost::asio::ip::tcp::socket socket);
    void loadJsonToEnvironment(const nlohmann::json& j, const std::string& prefix = "");
    
    /**
     * @brief Внутренний метод - создает адаптеры и вызывает виртуальный handleRequest
     */
    void handleBeastRequest(
        const boost::beast::http::request<boost::beast::http::string_body>& req,
        boost::beast::http::response<boost::beast::http::string_body>& res,
        const std::string& clientIp);
};