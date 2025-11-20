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
#include "settings/DbSettings.hpp"
#include "settings/CacheInvalidatorSettings.hpp"
#include <adapters/InMemoryRuleRepository.hpp>

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

    // Всё создаётся через DI
    auto injector = di::make_injector(
        di::bind<IEnvironment>().to(env_),
        di::bind<IDbSettings>().to<DbSettings>().in(di::singleton),
        di::bind<ICacheInvalidatorSettings>().to<CacheInvalidatorSettings>().in(di::singleton),
        //di::bind<IRuleRepository>().to<PostgreSQLRuleRepository>().in(di::singleton),
        di::bind<IRuleRepository>().to<InMemoryRuleRepository>().in(di::singleton),
        di::bind<ICacheInvalidator>().to<HttpCacheInvalidator>().in(di::singleton),
        di::bind<IRuleService>().to<RuleService>().in(di::singleton));

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

        // FIXME: это работает не так как надо
        // Сброс кэша должен быть в redirect-service
    handlers_[getHandlerKey("GET", "/cache/invalidate/*")] =
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();

    handlers_[getHandlerKey("GET", "/cache/invalidate")] =
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();

    std::cout << "[RuleServiceApp] DI injector configured, registered "
              << handlers_.size() << " handlers" << std::endl;
}

void RuleServiceApp::handleRequest(IRequest &req, IResponse &res)
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
        catch (const std::exception &e)
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
    const std::string &method,
    const std::string &path)
{
    std::string exactKey = getHandlerKey(method, path);
    auto it = handlers_.find(exactKey);
    if (it != handlers_.end())
    {
        return it->second;
    }

    for (const auto &[key, handler] : handlers_)
    {
        size_t methodDelimiter = key.find(':');
        if (methodDelimiter == std::string::npos)
            continue;

        std::string handlerMethod = key.substr(0, methodDelimiter);
        std::string handlerPattern = key.substr(methodDelimiter + 1);

        if (handlerMethod == method && RouteMatcher::matches(handlerPattern, path))
        {
            return handler;
        }
    }

    return nullptr;
}

std::string RuleServiceApp::getHandlerKey(const std::string &method, const std::string &pattern) const
{
    return method + ":" + pattern;
}