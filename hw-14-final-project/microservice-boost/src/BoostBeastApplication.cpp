#include "BoostBeastApplication.hpp"
#include "Environment.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <fstream>
#include <thread>

using json = nlohmann::json;

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

BoostBeastApplication::BoostBeastApplication()
    : running_(false)
{
    std::cout << "[App] BoostBeastApplication created" << std::endl;
}

BoostBeastApplication::~BoostBeastApplication()
{
    stop();
    std::cout << "[App] BoostBeastApplication destroyed" << std::endl;
}

void BoostBeastApplication::stop()
{
    if (running_)
    {
        std::cout << "[App] Stopping application..." << std::endl;
        running_ = false;
        
        if (acceptor_ && acceptor_->is_open())
        {
            acceptor_->close();
        }
        
        if (ioContext_)
        {
            ioContext_->stop();
        }
    }
}

void BoostBeastApplication::loadEnvironment(int argc, char* argv[])
{
    std::cout << "[BoostBeastApplication] Loading environment..." << std::endl;
    
    // Игнорируем argc/argv (можно расширить позже)
    (void)argc;
    (void)argv;
    
    // Создаем окружение
    env_ = std::make_shared<Environment>();
    
    // Пытаемся загрузить config.json
    try
    {
        std::ifstream configFile("config.json");
        
        if (!configFile.is_open())
        {
            std::cout << "[BoostBeastApplication] config.json not found" << std::endl;
            return;
        }
        
        std::cout << "[BoostBeastApplication] Reading config.json..." << std::endl;
        
        json config = json::parse(configFile);
        
        // Рекурсивно загружаем весь JSON в Environment
        loadJsonToEnvironment(config);
        
        std::cout << "[BoostBeastApplication] Configuration loaded from config.json" << std::endl;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "[BoostBeastApplication] JSON parse error: " << e.what() << std::endl;
        throw;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[BoostBeastApplication] Error loading config: " << e.what() << std::endl;
        throw;
    }
}

void BoostBeastApplication::loadJsonToEnvironment(const json& j, const std::string& prefix)
{
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        std::string key = prefix.empty() ? it.key() : prefix + "." + it.key();
        
        if (it->is_object())
        {
            // Рекурсивно обрабатываем вложенные объекты
            loadJsonToEnvironment(*it, key);
        }
        else if (it->is_string())
        {
            env_->setProperty(key, it->get<std::string>());
        }
        else if (it->is_number_integer())
        {
            env_->setProperty(key, it->get<int>());
        }
        else if (it->is_number_unsigned())
        {
            env_->setProperty(key, static_cast<int>(it->get<unsigned int>()));
        }
        else if (it->is_boolean())
        {
            env_->setProperty(key, it->get<bool>());
        }
        else if (it->is_number_float())
        {
            env_->setProperty(key, it->get<double>());
        }
        else if (it->is_array())
        {
            // Массивы пока игнорируем (можно расширить)
            std::cout << "[BoostBeastApplication] Skipping array: " << key << std::endl;
        }
        else if (it->is_null())
        {
            // null игнорируем
        }
    }
}

void BoostBeastApplication::start()
{
    try
    {
        // Получаем конфигурацию из окружения
        std::string host = env_->get<std::string>("server.host");
        int port = env_->get<int>("server.port");
        
        std::cout << "[App] Starting HTTP server..." << std::endl;
        
        // Создаем IO контекст
        ioContext_ = std::make_unique<asio::io_context>();

        // Создаем endpoint
        auto const address = asio::ip::make_address(host);
        tcp::endpoint endpoint{address, static_cast<unsigned short>(port)};

        // Создаем acceptor
        acceptor_ = std::make_unique<tcp::acceptor>(*ioContext_, endpoint);

        std::cout << "[Server] Listening on " << host << ":" << port << std::endl;
        std::cout << "[Server] Server is ready to accept connections!" << std::endl;

        running_ = true;

        // Accept loop
        while (running_)
        {
            tcp::socket socket{*ioContext_};
            acceptor_->accept(socket);

            std::cout << "[Server] New connection accepted" << std::endl;

            std::thread([this](tcp::socket socket) {
                handleSession(std::move(socket));
            }, std::move(socket)).detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Server] Error: " << e.what() << std::endl;
        running_ = false;
    }
}

void BoostBeastApplication::handleSession(tcp::socket socket)
{
    try
    {
        // Извлекаем IP клиента из сокета
        std::string clientIp = "0.0.0.0";
        try
        {
            auto endpoint = socket.remote_endpoint();
            clientIp = endpoint.address().to_string();
            std::cout << "[Session] Client connected from: " << clientIp << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Session] Failed to get client IP: " << e.what() << std::endl;
        }
        
        beast::flat_buffer buffer;

        // Читаем HTTP запрос
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        std::cout << "[Session] Received request: " 
                  << req.method_string() << " " << req.target() << std::endl;

        // Создаем HTTP ответ
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "BoostBeast");
        res.keep_alive(req.keep_alive());

        // Передаем в переопределенный метод подкласса с IP клиента
        handleRequest(req, res, clientIp);

        // Отправляем ответ
        http::write(socket, res);

        std::cout << "[Session] Response sent with status: " 
                  << res.result_int() << std::endl;

        // Закрываем соединение
        beast::error_code ec;
        socket.shutdown(tcp::socket::shutdown_send, ec);

        if (ec && ec != beast::errc::not_connected)
        {
            std::cerr << "[Session] Shutdown error: " << ec.message() << std::endl;
        }
    }
    catch (const beast::system_error& se)
    {
        if (se.code() != http::error::end_of_stream &&
            se.code() != beast::errc::not_connected)
        {
            std::cerr << "[Session] Error: " << se.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Session] Unexpected error: " << e.what() << std::endl;
    }
}