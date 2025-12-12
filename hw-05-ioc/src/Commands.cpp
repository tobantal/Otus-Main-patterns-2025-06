#include "Commands.hpp"
#include <iostream>

RegisterCommand::RegisterCommand(const std::string& key, IocContainer::FactoryFunction factory)
    : key_(key), factory_(std::move(factory)) {
}

void RegisterCommand::execute() {
    std::cout << ">>> RegisterCommand::execute() for key: '" << key_ << "'" << std::endl;
    
    auto currentScope = IocContainer::getInstance().getCurrentScope();
    if (currentScope) {
        // Регистрируем в текущем скоупе
        currentScope->registerDependency(key_, factory_);
    } else {
        // Регистрируем глобально
        IocContainer::getInstance().registerGlobalDependency(key_, factory_);
    }
    
    std::cout << ">>> RegisterCommand::execute() completed" << std::endl;
}

CreateScopeCommand::CreateScopeCommand(const std::string& scopeId)
    : scopeId_(scopeId) {
}

void CreateScopeCommand::execute() {
    IocContainer::getInstance().createScope(scopeId_);
}

SetCurrentScopeCommand::SetCurrentScopeCommand(const std::string& scopeId)
    : scopeId_(scopeId) {
}

void SetCurrentScopeCommand::execute() {
    IocContainer::getInstance().setCurrentScope(scopeId_);
}