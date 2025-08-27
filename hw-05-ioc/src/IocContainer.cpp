#include "IocContainer.hpp"
#include <stdexcept>

thread_local std::shared_ptr<Scope> IocContainer::currentScope_ = nullptr;

IocContainer& IocContainer::getInstance() {
    static IocContainer instance;
    return instance;
}

void IocContainer::registerGlobalDependency(const std::string& key, FactoryFunction factory) {
    std::lock_guard<std::recursive_mutex> lock(globalMutex_);
    globalDependencies_[key] = std::move(factory);
}

std::shared_ptr<Scope> IocContainer::createScope(const std::string& scopeId) {
    std::lock_guard<std::recursive_mutex> lock(globalMutex_);
    
    auto scope = std::make_shared<Scope>();
    scopes_[scopeId] = scope;
    
    return scope;
}

void IocContainer::setCurrentScope(const std::string& scopeId) {
    std::lock_guard<std::recursive_mutex> lock(globalMutex_);
    
    auto it = scopes_.find(scopeId);
    if (it == scopes_.end()) {
        throw std::runtime_error("Scope not found: " + scopeId);
    }
    
    currentScope_ = it->second;
}

std::shared_ptr<Scope> IocContainer::getCurrentScope() const {
    return currentScope_;
}