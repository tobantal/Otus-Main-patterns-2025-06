#include "MultiRegionCollisionSystem.hpp"
#include "UpdateRegionCommand.hpp"
#include <stdexcept>

MultiRegionCollisionSystem::MultiRegionCollisionSystem(
    double cellSize,
    std::shared_ptr<ICollisionChecker> collisionChecker,
    int numSystems)
    : collisionChecker_(std::move(collisionChecker))
    , cellSize_(cellSize)
    , numSystems_(numSystems)
{
    if (cellSize <= 0) {
        throw std::invalid_argument("Cell size must be positive");
    }
    if (numSystems < 1) {
        throw std::invalid_argument("Number of systems must be at least 1");
    }
    if (!collisionChecker_) {
        throw std::invalid_argument("CollisionChecker cannot be null");
    }
    
    // Создаём менеджеры с разным смещением
    // Смещение i-й системы: (i/N) * cellSize
    for (int i = 0; i < numSystems; ++i) {
        double offsetFraction = static_cast<double>(i) / numSystems;
        Vector2D offset(cellSize * offsetFraction, cellSize * offsetFraction);
        
        auto resolver = std::make_shared<GridRegionResolver>(cellSize, offset);
        managers_.push_back(std::make_shared<RegionManager>(resolver));
    }
    
    // Инициализируем макрокоманды как nullptr
    collisionMacros_.resize(numSystems, nullptr);
}

void MultiRegionCollisionSystem::addObject(std::shared_ptr<IGameObject> object) {
    if (!object) {
        return;
    }
    
    // Добавляем объект во все системы
    for (auto& manager : managers_) {
        manager->addObject(object);
    }
}

void MultiRegionCollisionSystem::removeObject(std::shared_ptr<IGameObject> object) {
    if (!object) {
        return;
    }
    
    // Удаляем объект из всех систем
    for (auto& manager : managers_) {
        manager->removeObject(object);
    }
}

void MultiRegionCollisionSystem::updateObject(std::shared_ptr<IGameObject> object) {
    if (!object) {
        return;
    }
    
    // Обновляем объект в каждой системе
    for (size_t i = 0; i < managers_.size(); ++i) {
        // Создаём команду обновления для данной системы
        UpdateRegionCommand updateCmd(
            object,
            managers_[i],
            collisionChecker_,
            collisionMacros_[i]
        );
        
        // Выполняем обновление
        updateCmd.execute();
    }
}

std::vector<std::shared_ptr<ICommand>> 
MultiRegionCollisionSystem::getCollisionCommands() const {
    std::vector<std::shared_ptr<ICommand>> result;
    
    // Собираем команды из всех систем
    for (const auto& macro : collisionMacros_) {
        if (macro) {
            result.push_back(macro);
        }
    }
    
    return result;
}

int MultiRegionCollisionSystem::getNumSystems() const {
    return numSystems_;
}

std::shared_ptr<RegionManager> MultiRegionCollisionSystem::getManager(int index) const {
    if (index < 0 || index >= numSystems_) {
        throw std::out_of_range("System index out of range");
    }
    return managers_[index];
}