#include "Scope.hpp"

Scope::Scope(std::shared_ptr<Scope> parent) 
    : parent_(std::move(parent)) {
}

void Scope::registerDependency(const std::string& key, FactoryFunction factory) {
    std::lock_guard<std::mutex> lock(mutex_);
    dependencies_[key] = std::move(factory);
}

std::shared_ptr<Scope> Scope::getParent() const {
    return parent_;
}