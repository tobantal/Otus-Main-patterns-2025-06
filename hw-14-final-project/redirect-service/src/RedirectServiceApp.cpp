#include "RedirectServiceApp.hpp"
#include "handlers/GetUserHandler.hpp"
#include "services/IUserService.hpp"
#include "services/UserService.hpp"
#include "repositories/IUserRepository.hpp"
#include "repositories/InMemoryUserRepository.hpp"
#include "BeastRequestAdapter.hpp"
#include "BeastResponseAdapter.hpp"
#include <iostream>

RedirectServiceApp::RedirectServiceApp(const ServerConfig& config)
    : BoostBeastApplication(config)
{
    std::cout << "[RedirectServiceApp] Application created" << std::endl;
}

RedirectServiceApp::~RedirectServiceApp()
{
    std::cout << "[RedirectServiceApp] Application destroyed" << std::endl;
}

void RedirectServiceApp::configureInjection()
{
    std::cout << "[RedirectServiceApp] Configuring DI injector..." << std::endl;
    
    // Создаем Boost.DI injector
    auto injector = di::make_injector(
        di::bind<IUserRepository>().to<InMemoryUserRepository>().in(di::singleton),
        di::bind<IUserService>().to<UserService>().in(di::singleton)
    );
    
    // Создаем хендлеры через DI и регистрируем в map
    handlers_["GET:/api/users"] = injector.create<std::shared_ptr<GetUserHandler>>();
    
    std::cout << "[RedirectServiceApp] DI injector configured, registered " 
              << handlers_.size() << " handlers" << std::endl;
}

void RedirectServiceApp::configureRoutes()
{
    std::cout << "[RedirectServiceApp] Configuring routes..." << std::endl;
    std::cout << "[RedirectServiceApp] Routes configured" << std::endl;
}

void RedirectServiceApp::handleRequest(
    const boost::beast::http::request<boost::beast::http::string_body>& beast_req,
    boost::beast::http::response<boost::beast::http::string_body>& beast_res)
{
    // Извлекаем метод и путь
    std::string method = std::string(beast_req.method_string());
    std::string target(beast_req.target());
    
    // Убираем query string из пути
    std::string path = target;
    size_t queryPos = path.find('?');
    if (queryPos != std::string::npos)
    {
        path = path.substr(0, queryPos);
    }
    
    // Убираем ID из пути (/api/users/123 -> /api/users)
    if (path.find("/api/users/") == 0)
    {
        path = "/api/users";
    }
    
    // Формируем ключ
    std::string key = method + ":" + path;
    
    std::cout << "[RedirectServiceApp] Route key: " << key << std::endl;
    
    // Ищем хендлер в map
    auto it = handlers_.find(key);
    
    if (it != handlers_.end())
    {
        // Создаем адаптеры
        BeastRequestAdapter req(beast_req);
        BeastResponseAdapter res(beast_res);
        
        // Вызываем хендлер
        it->second->handle(req, res);
        
        beast_res.prepare_payload();
    }
    else
    {
        std::cout << "[RedirectServiceApp] Handler not found for: " << key << std::endl;
        
        beast_res.result(boost::beast::http::status::not_found);
        beast_res.set(boost::beast::http::field::content_type, "text/plain");
        beast_res.body() = "404 Not Found";
        beast_res.prepare_payload();
    }
}