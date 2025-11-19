#pragma once
#include "BoostBeastApplication.hpp"
#include "IHttpHandler.hpp"
#include "ports/IRuleRepository.hpp"
#include "ports/ICacheInvalidator.hpp"
#include <memory>
#include <map>
#include <string>

class RuleServiceApp : public BoostBeastApplication
{
public:
    RuleServiceApp();
    ~RuleServiceApp() override;

    void configureInjection() override;

protected:
    void handleRequest(IRequest& req, IResponse& res) override;

private:
    std::map<std::string, std::shared_ptr<IHttpHandler>> handlers_;
    
    std::shared_ptr<IHttpHandler> findHandler(const std::string& method, const std::string& path);
    std::string getHandlerKey(const std::string& method, const std::string& pattern) const;
};