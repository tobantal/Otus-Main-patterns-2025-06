#include "Commands.hpp"
#include <iostream>

RegisterCommand::RegisterCommand(const std::string& key, IocContainer::FactoryFunction factory)
    : key_(key), factory_(std::move(factory)) {
}

void RegisterCommand::execute() {
    //IocContainer::getInstance().registerGlobalDependency(key_, factory_);

    std::cout << ">>> RegisterCommand::execute() for key: '" << key_ << "'" << std::endl;
    IocContainer::getInstance().registerGlobalDependency(key_, factory_);
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