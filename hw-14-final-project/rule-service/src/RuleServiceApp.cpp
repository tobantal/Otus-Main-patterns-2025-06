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
#include "ports/IRuleRepository.hpp"
#include "ports/ICacheInvalidator.hpp"

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
        di::bind<IRuleRepository>().to<PostgreSQLRuleRepository>().in(di::singleton),
        //di::bind<IRuleRepository>().to<InMemoryRuleRepository>().in(di::singleton),
        di::bind<ICacheInvalidatorSettings>().to<CacheInvalidatorSettings>().in(di::singleton),
        di::bind<ICacheInvalidator>().to<HttpCacheInvalidator>().in(di::singleton),
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

        // FIXME: это работает не так как надо
        // Сброс кэша должен быть в redirect-service
    handlers_[getHandlerKey("GET", "/cache/invalidate/*")] =
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();

    handlers_[getHandlerKey("GET", "/cache/invalidate")] =
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();

    std::cout << "[RuleServiceApp] DI injector configured, registered "
              << handlers_.size() << " handlers" << std::endl;
}