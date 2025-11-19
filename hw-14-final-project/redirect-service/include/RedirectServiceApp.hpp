#pragma once
#include "BoostBeastApplication.hpp"
#include "IHttpHandler.hpp"
#include "services/RedirectService.hpp"
#include "ports/IRuleClient.hpp"
#include <memory>
#include <map>
#include <string>

/**
 * @file RedirectServiceApp.hpp
 * @brief Главное приложение redirect-service
 * @author Anton Tobolkin
 */

/**
 * @class RedirectServiceApp
 * @brief Веб-приложение для обработки редиректов
 */
class RedirectServiceApp : public BoostBeastApplication
{
public:
    RedirectServiceApp();
    ~RedirectServiceApp() override;
    
    void loadEnvironment(int argc, char* argv[]) override;
    void configureInjection() override;

protected:
    /**
     * @brief Обработать HTTP-запрос (работа только с абстракциями)
     */
    void handleRequest(IRequest& req, IResponse& res) override;

private:
    std::shared_ptr<RedirectService> redirectService_;
    std::shared_ptr<IRuleClient> ruleClient_;
    std::map<std::string, std::shared_ptr<IHttpHandler>> handlers_;
};