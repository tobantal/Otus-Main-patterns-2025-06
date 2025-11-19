#include "RedirectServiceApp.hpp"
#include "Environment.hpp"
#include "BeastRequestAdapter.hpp"
#include "BeastResponseAdapter.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include "adapters/InMemoryRuleClient.hpp"
#include "services/RedirectService.hpp"
#include "ports/IRedirectService.hpp"
#include "ports/IRuleClient.hpp"
#include "ports/IRuleEvaluator.hpp"
#include "handlers/RedirectHandler.hpp"
#include "RouteMatcher.hpp"
#include "services/DSLEvaluator.hpp"

/**
 * @file RedirectServiceApp.cpp
 * @brief Реализация главного класса приложения
 * @author Anton Tobolkin
 */

using json = nlohmann::json;

RedirectServiceApp::RedirectServiceApp()
{
    std::cout << "[RedirectServiceApp] Application created" << std::endl;
}

RedirectServiceApp::~RedirectServiceApp()
{
    std::cout << "[RedirectServiceApp] Application destroyed" << std::endl;
}

void RedirectServiceApp::loadEnvironment(int argc, char* argv[])
{
    std::cout << "[RedirectServiceApp] Loading environment..." << std::endl;
    
    // Вызываем родительский метод для загрузки config.json
    BoostBeastApplication::loadEnvironment(argc, argv);
    
    // Валидируем обязательные параметры и устанавливаем дефолты
    try
    {
        // Проверяем server.host
        env_->get<std::string>("server.host");
    }
    catch (const std::exception&)
    {
        std::cout << "[RedirectServiceApp] server.host not found, using default: 0.0.0.0" << std::endl;
        env_->setProperty("server.host", std::string("0.0.0.0"));
    }
    
    try
    {
        // Проверяем server.port
        env_->get<int>("server.port");
    }
    catch (const std::exception&)
    {
        std::cout << "[RedirectServiceApp] server.port not found, using default: 8080" << std::endl;
        env_->setProperty("server.port", 8080);
    }
    
    try
    {
        // Проверяем cache.enabled
        env_->get<bool>("cache.enabled");
    }
    catch (const std::exception&)
    {
        std::cout << "[RedirectServiceApp] cache.enabled not found, using default: true" << std::endl;
        env_->setProperty("cache.enabled", true);
    }
    
    try
    {
        // Проверяем cache.ttl
        env_->get<int>("cache.ttl");
    }
    catch (const std::exception&)
    {
        std::cout << "[RedirectServiceApp] cache.ttl not found, using default: 3600" << std::endl;
        env_->setProperty("cache.ttl", 3600);
    }
    
    try
    {
        // Проверяем services.rule_service_url
        env_->get<std::string>("services.rule_service_url");
    }
    catch (const std::exception&)
    {
        std::cout << "[RedirectServiceApp] services.rule_service_url not found, using default: http://localhost:8081" << std::endl;
        env_->setProperty("services.rule_service_url", std::string("http://localhost:8081"));
    }
    
    // Выводим загруженную конфигурацию
    std::cout << "[RedirectServiceApp] Environment loaded:" << std::endl;
    std::cout << "  server.host: " << env_->get<std::string>("server.host") << std::endl;
    std::cout << "  server.port: " << env_->get<int>("server.port") << std::endl;
    std::cout << "  cache.enabled: " << (env_->get<bool>("cache.enabled") ? "true" : "false") << std::endl;
    std::cout << "  cache.ttl: " << env_->get<int>("cache.ttl") << std::endl;
    std::cout << "  services.rule_service_url: " << env_->get<std::string>("services.rule_service_url") << std::endl;
}

void RedirectServiceApp::configureInjection()
{
    std::cout << "[RedirectServiceApp] Configuring DI injector..." << std::endl;
    
    // Создаем Boost.DI injector
    auto injector = di::make_injector(
        di::bind<IRuleClient>().to<InMemoryRuleClient>().in(di::singleton),
        di::bind<IRuleEvaluator>().to<DSLEvaluator>().in(di::singleton),
        di::bind<IRedirectService>().to<RedirectService>().in(di::singleton)
    );
    
    // Регистрируем хендлеры
    handlers_["GET:/r/*"] = injector.create<std::shared_ptr<RedirectHandler>>();
    
    std::cout << "[RedirectServiceApp] DI injector configured, registered " 
              << handlers_.size() << " handlers" << std::endl;
}

void RedirectServiceApp::handleRequest(
    const boost::beast::http::request<boost::beast::http::string_body>& beast_req,
    boost::beast::http::response<boost::beast::http::string_body>& beast_res)
{
    std::string method = std::string(beast_req.method_string());
    std::string target(beast_req.target());
    
    // Убираем query string
    std::string path = target;
    size_t queryPos = path.find('?');
    if (queryPos != std::string::npos)
    {
        path = path.substr(0, queryPos);
    }
    
    std::string requestKey = method + ":" + path;
    
    std::cout << "[RedirectServiceApp] Matching: " << requestKey << std::endl;
    
    // Ищем подходящий паттерн
    for (const auto& [pattern, handler] : handlers_)
    {
        // Извлекаем путь из паттерна (убираем метод)
        size_t colonPos = pattern.find(':');
        if (colonPos == std::string::npos)
        {
            continue;
        }
        
        std::string patternMethod = pattern.substr(0, colonPos);
        std::string patternPath = pattern.substr(colonPos + 1);
        
        // Проверяем метод
        if (patternMethod != method)
        {
            continue;
        }
        
        // Проверяем путь с помощью RouteMatcher
        if (RouteMatcher::matches(patternPath, path))
        {
            std::cout << "[RedirectServiceApp] Matched pattern: " << pattern << std::endl;
            
            BeastRequestAdapter req(beast_req);
            BeastResponseAdapter res(beast_res);
            handler->handle(req, res);
            beast_res.prepare_payload();
            return;
        }
    }
    
    std::cout << "[RedirectServiceApp] No matching handler for: " << requestKey << std::endl;
    beast_res.result(boost::beast::http::status::not_found);
    beast_res.set(boost::beast::http::field::content_type, "text/plain");
    beast_res.body() = "404 Not Found";
    beast_res.prepare_payload();
}