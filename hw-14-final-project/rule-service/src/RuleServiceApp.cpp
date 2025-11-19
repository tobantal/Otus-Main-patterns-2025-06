#include "RuleServiceApp.hpp"
#include "adapters/PostgreSQLRuleRepository.hpp"
#include "adapters/HttpCacheInvalidator.hpp"
#include "services/RuleService.hpp"
#include "ports/IRuleService.hpp"
#include "handlers/CreateRuleHandler.hpp"
#include "handlers/GetRuleHandler.hpp"
#include "handlers/ListRulesHandler.hpp"
#include "handlers/UpdateRuleHandler.hpp"
#include "handlers/DeleteRuleHandler.hpp"
#include "handlers/InvalidateCacheHandler.hpp"
#include "RouteMatcher.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"
#include <boost/di.hpp>
#include <iostream>

namespace di = boost::di;

/**
 * @file RuleServiceApp.cpp
 * @brief Реализация главного приложения rule-service
 * @author Anton Tobolkin
 */

RuleServiceApp::RuleServiceApp()
{
    std::cout << "[RuleServiceApp] Application created" << std::endl;
}

RuleServiceApp::~RuleServiceApp()
{
    std::cout << "[RuleServiceApp] Application destroyed" << std::endl;
}

void RuleServiceApp::configureInjection()
{
    std::cout << "[RuleServiceApp] Configuring DI injector..." << std::endl;

    // TODO: создаем Environment как биндинг в DI
    
    // Получаем параметры из окружения
    // FIXME: вся это логика должна быть в RepositoryConfig (см. ниже)
    std::string dbHost = env_->get<std::string>("db.host");
    int dbPort = env_->get<int>("db.port");
    std::string dbName = env_->get<std::string>("db.name");
    std::string dbUser = env_->get<std::string>("db.user");
    std::string dbPassword = env_->get<std::string>("db.password");
    std::string connectionString = 
        "host=" + dbHost + 
        " port=" + std::to_string(dbPort) +
        " dbname=" + dbName +
        " user=" + dbUser +
        " password=" + dbPassword;
    
    std::string redirectServiceUrl = env_->get<std::string>("redirect_service.url");
    
    std::cout << "[RuleServiceApp] Database: " << dbHost << ":" << dbPort << "/" << dbName << std::endl;
    std::cout << "[RuleServiceApp] Redirect service: " << redirectServiceUrl << std::endl;
    
    // Создаем адаптеры с параметрами (они не могут быть созданы DI без параметров)
    // FIXME: добавить адаптеры, на самом деле могут быть созданы
    // PostgreSQLRuleRepository принимает RepositoryConfig, который в конструктор принимает Environment
    // RepositoryConfig - адаптер, который может быть создан DI
    // DI работает так: Environment -> RepositoryConfig -> PostgreSQLRuleRepository
    auto repository = std::make_shared<PostgreSQLRuleRepository>(connectionString);
    // FIXME: в HttpCacheInvalidator инжектим CacheInvalidatorConfig(Environment)
    auto cacheInvalidator = std::make_shared<HttpCacheInvalidator>(redirectServiceUrl);
    
    // Создаем Boost.DI injector с instances
    auto injector = di::make_injector(
        di::bind<IRuleRepository>().to(repository),
        di::bind<ICacheInvalidator>().to(cacheInvalidator),
        di::bind<IRuleService>().to<RuleService>().in(di::singleton)
    );
    
    // Регистрируем хендлеры через injector
    handlers_[getHandlerKey("POST", "/rules")] = 
        injector.create<std::shared_ptr<CreateRuleHandler>>();
    
    handlers_[getHandlerKey("GET", "/rules/*")] = 
        injector.create<std::shared_ptr<GetRuleHandler>>();
    
    handlers_[getHandlerKey("GET", "/rules")] = 
        injector.create<std::shared_ptr<ListRulesHandler>>();
    
    handlers_[getHandlerKey("PUT", "/rules/*")] = 
        injector.create<std::shared_ptr<UpdateRuleHandler>>();
    
    handlers_[getHandlerKey("DELETE", "/rules/*")] = 
        injector.create<std::shared_ptr<DeleteRuleHandler>>();
    
    handlers_[getHandlerKey("GET", "/cache/invalidate/*")] = 
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();
    
    handlers_[getHandlerKey("GET", "/cache/invalidate")] = 
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();
    
    std::cout << "[RuleServiceApp] DI injector configured, registered " 
              << handlers_.size() << " handlers" << std::endl;
}

void RuleServiceApp::handleRequest(IRequest& req, IResponse& res)
{
    std::string path = req.getPath();
    std::string method = req.getMethod();
    
    std::cout << "[RuleServiceApp] " << method << " " << path 
              << " from " << req.getClientIp() << std::endl;
    
    auto handler = findHandler(method, path);
    
    if (handler)
    {
        try
        {
            handler->handle(req, res);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[RuleServiceApp] Handler error: " << e.what() << std::endl;
            res.setStatus(500);
            res.setHeader("Content-Type", "application/json");
            res.setBody(R"({"error": "Internal server error"})");
        }
    }
    else
    {
        std::cout << "[RuleServiceApp] No handler found" << std::endl;
        
        res.setStatus(404);
        res.setHeader("Content-Type", "application/json");
        res.setBody(R"({"error": "Not found"})");
    }
}

std::shared_ptr<IHttpHandler> RuleServiceApp::findHandler(
    const std::string& method, 
    const std::string& path)
{
    std::string exactKey = getHandlerKey(method, path);
    auto it = handlers_.find(exactKey);
    if (it != handlers_.end())
    {
        return it->second;
    }
    
    for (const auto& [key, handler] : handlers_)
    {
        size_t methodDelimiter = key.find(':');
        if (methodDelimiter == std::string::npos) continue;
        
        std::string handlerMethod = key.substr(0, methodDelimiter);
        std::string handlerPattern = key.substr(methodDelimiter + 1);
        
        if (handlerMethod == method && RouteMatcher::matches(handlerPattern, path))
        {
            return handler;
        }
    }
    
    return nullptr;
}

std::string RuleServiceApp::getHandlerKey(const std::string& method, const std::string& pattern) const
{
    return method + ":" + pattern;
}