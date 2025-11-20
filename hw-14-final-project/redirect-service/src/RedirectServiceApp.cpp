#include "RedirectServiceApp.hpp"
#include <boost/di.hpp>
#include "adapters/HttpRuleClient.hpp"
#include "services/RedirectService.hpp"
#include "ports/IRedirectService.hpp"
#include "ports/IRuleClient.hpp"
#include "ports/IRuleEvaluator.hpp"
#include "handlers/RedirectHandler.hpp"
#include "RouteMatcher.hpp"
#include "services/DSLEvaluator.hpp"
#include "IEnvironment.hpp"
#include "settings/IRuleServiceSettings.hpp"
#include "settings/RuleServiceSettings.hpp"
#include "IHttpClient.hpp"
#include "BeastRequestAdapter.hpp"
#include <Environment.hpp>
#include <HttpClient.hpp>
#include "cache/RulesCache.hpp"
#include "handlers/InvalidateCacheHandler.hpp"
#include "handlers/InvalidateCacheByKeyHandler.hpp"


namespace di = boost::di;

/**
 * @file RedirectServiceApp.cpp
 * @brief Реализация главного класса приложения
 * @author Anton Tobolkin
 */

RedirectServiceApp::RedirectServiceApp()
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

    auto injector = di::make_injector(
        di::bind<IEnvironment>().to(env_),
        di::bind<IRulesCache>().to<RulesCache>().in(di::singleton),
        di::bind<IRuleServiceSettings>().to<RuleServiceSettings>().in(di::singleton),
        di::bind<IHttpClient>().to<HttpClient>().in(di::singleton),
        di::bind<IRuleClient>().to<HttpRuleClient>().in(di::singleton),
        di::bind<IRuleEvaluator>().to<DSLEvaluator>().in(di::singleton),
        di::bind<IRedirectService>().to<RedirectService>().in(di::singleton));

    handlers_[getHandlerKey("GET", "/r/*")] =
        injector.create<std::shared_ptr<RedirectHandler>>();

    handlers_[getHandlerKey("DELETE", "/cache/invalidate")] =
        injector.create<std::shared_ptr<InvalidateCacheHandler>>();

    handlers_[getHandlerKey("DELETE", "/cache/invalidate/*")] =
        injector.create<std::shared_ptr<InvalidateCacheByKeyHandler>>();

    std::cout << "[RedirectServiceApp] DI injector configured, registered "
              << handlers_.size() << " handlers" << std::endl;
}
