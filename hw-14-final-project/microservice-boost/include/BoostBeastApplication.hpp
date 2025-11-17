#pragma once

#include "IWebApplication.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>
#include <atomic>

namespace asio = boost::asio;

/**
 * @file BoostBeastApplication.hpp
 * @brief Реализация веб-приложения на Boost.Beast
 * @author Anton Tobolkin
 */

/**
 * @class BoostBeastApplication
 * @brief Базовый класс для веб-приложений на Boost.Beast
 * 
 * Реализует Template Method паттерн из IWebApplication.
 */
class BoostBeastApplication : public IWebApplication
{
public:
    BoostBeastApplication();
    virtual ~BoostBeastApplication();

    /**
     * @brief Остановить веб-приложение
     */
    void stop();

protected:
    /**
     * @brief Запустить HTTP сервер
     * 
     * Получает host и port из env_
     */
    void start() override;

private:
    std::unique_ptr<asio::io_context> ioContext_;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
    std::atomic<bool> running_;

    /**
     * @brief Обработать HTTP сессию
     */
    void handleSession(asio::ip::tcp::socket socket);
    
    /**
     * @brief Обработать HTTP запрос (переопределяется в подклассах)
     */
    virtual void handleRequest(
        const boost::beast::http::request<boost::beast::http::string_body>& req,
        boost::beast::http::response<boost::beast::http::string_body>& res) = 0;
};