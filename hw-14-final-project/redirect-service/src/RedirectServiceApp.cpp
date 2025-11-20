#include "RedirectServiceApp.hpp"
#include <boost/di.hpp>
#include "adapters/InMemoryRuleClient.hpp"
#include "services/RedirectService.hpp"
#include "ports/IRedirectService.hpp"
#include "ports/IRuleClient.hpp"
#include "ports/IRuleEvaluator.hpp"
#include "handlers/RedirectHandler.hpp"
#include "RouteMatcher.hpp"
#include "services/DSLEvaluator.hpp"

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

    // Создаем Boost.DI injector
    auto injector = di::make_injector(
        di::bind<IRuleClient>().to<InMemoryRuleClient>().in(di::singleton),
        di::bind<IRuleEvaluator>().to<DSLEvaluator>().in(di::singleton),
        di::bind<IRedirectService>().to<RedirectService>().in(di::singleton));

    // Регистрируем хендлеры через injector
    handlers_[getHandlerKey("GET", "/r/*")] = injector.create<std::shared_ptr<RedirectHandler>>();

    std::cout << "[RedirectServiceApp] DI injector configured, registered "
              << handlers_.size() << " handlers" << std::endl;
}