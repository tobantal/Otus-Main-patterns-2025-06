#include "RegionManager.hpp"
#include <stdexcept>

RegionManager::RegionManager(std::shared_ptr<IRegionResolver> resolver)
    : resolver_(std::move(resolver)) 
{
    if (!resolver_) {
        throw std::invalid_argument("Resolver cannot be null");
    }
}

void RegionManager::addObject(std::shared_ptr<IGameObject> object) {
    if (!object) {
        throw std::invalid_argument("Object cannot be null");
    }
    
    // Определяем регион для объекта
    RegionId region = resolver_->getRegion(*object);
    
    // Добавляем в соответствующий регион
    regions_[region].insert(object);
}

void RegionManager::removeObject(std::shared_ptr<IGameObject> object) {
    if (!object) {
        return;
    }
    
    // Определяем текущий регион объекта
    RegionId region = resolver_->getRegion(*object);
    
    // Удаляем из региона
    auto it = regions_.find(region);
    if (it != regions_.end()) {
        it->second.erase(object);
        
        // Удаляем пустой регион
        if (it->second.empty()) {
            regions_.erase(it);
        }
    }
}

void RegionManager::moveObject(std::shared_ptr<IGameObject> object,
                               const RegionId& oldRegion,
                               const RegionId& newRegion) {
    if (!object) {
        return;
    }
    
    // Если регионы совпадают, ничего не делаем
    if (oldRegion == newRegion) {
        return;
    }
    
    // Удаляем из старого региона
    auto oldIt = regions_.find(oldRegion);
    if (oldIt != regions_.end()) {
        oldIt->second.erase(object);
        if (oldIt->second.empty()) {
            regions_.erase(oldIt);
        }
    }
    
    // Добавляем в новый регион
    regions_[newRegion].insert(object);
}

std::vector<std::shared_ptr<IGameObject>> 
RegionManager::getObjectsInRegion(const RegionId& region) const {
    auto it = regions_.find(region);
    if (it == regions_.end()) {
        return {};  // Пустой регион
    }
    
    // Копируем объекты в вектор
    return std::vector<std::shared_ptr<IGameObject>>(
        it->second.begin(), it->second.end()
    );
}

const IRegionResolver& RegionManager::getResolver() const {
    return *resolver_;
}

size_t RegionManager::getRegionCount() const {
    return regions_.size();
}