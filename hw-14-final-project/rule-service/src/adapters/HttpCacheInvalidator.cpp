#include "adapters/HttpCacheInvalidator.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

/**
 * @file HttpCacheInvalidator.cpp
 * @brief Реализация HTTP-клиента для инвалидации кэша
 * @author Anton Tobolkin
 */

HttpCacheInvalidator::HttpCacheInvalidator(std::shared_ptr<ICacheInvalidatorSettings> settings)
    : redirectServiceUrl_(settings->getRedirectServiceUrl())
{
    std::cout << "[HttpCacheInvalidator] Created with redirect-service URL: " 
              << redirectServiceUrl_ << std::endl;
}

bool HttpCacheInvalidator::invalidate(const std::string& shortId)
{
    std::string path = "/cache/invalidate/" + shortId;
    std::cout << "[HttpCacheInvalidator] Invalidating cache for: " << shortId << std::endl;
    return sendGetRequest(path);
}

bool HttpCacheInvalidator::invalidateAll()
{
    std::cout << "[HttpCacheInvalidator] Invalidating all cache" << std::endl;
    return sendGetRequest("/cache/invalidate");
}

bool HttpCacheInvalidator::sendGetRequest(const std::string& path) const
{
    try
    {
        // Парсим URL (предполагаем формат http://host:port)
        std::string host;
        std::string port;
        
        // Упрощенный парсинг URL
        size_t protocolEnd = redirectServiceUrl_.find("://");
        if (protocolEnd == std::string::npos)
        {
            std::cerr << "[HttpCacheInvalidator] Invalid URL format" << std::endl;
            return false;
        }
        
        std::string urlWithoutProtocol = redirectServiceUrl_.substr(protocolEnd + 3);
        size_t portDelimiter = urlWithoutProtocol.find(':');
        
        if (portDelimiter != std::string::npos)
        {
            host = urlWithoutProtocol.substr(0, portDelimiter);
            port = urlWithoutProtocol.substr(portDelimiter + 1);
        }
        else
        {
            host = urlWithoutProtocol;
            port = "80";
        }
        
        std::cout << "[HttpCacheInvalidator] Sending GET " << host << ":" << port << path << std::endl;
        
        // Создаем IO context
        asio::io_context ioc;
        
        // Резолвим хост
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(host, port);
        
        // Создаем и подключаем сокет
        beast::tcp_stream stream(ioc);
        stream.connect(results);
        
        // Формируем HTTP GET запрос
        http::request<http::string_body> req{http::verb::get, path, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "rule-service/1.0");
        
        // Отправляем запрос
        http::write(stream, req);
        
        // Читаем ответ
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);
        
        // Закрываем соединение
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        
        // Проверяем статус ответа
        bool success = (res.result() == http::status::ok);
        
        if (success)
        {
            std::cout << "[HttpCacheInvalidator] Cache invalidation successful" << std::endl;
        }
        else
        {
            std::cout << "[HttpCacheInvalidator] Cache invalidation failed with status: " 
                      << res.result_int() << std::endl;
        }
        
        return success;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[HttpCacheInvalidator] Error sending request: " << e.what() << std::endl;
        return false;
    }
}