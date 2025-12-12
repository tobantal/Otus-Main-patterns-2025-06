#include "UpdateRegionCommand.hpp"
#include "CheckCollisionCommand.hpp"
#include <stdexcept>
#include <vector>

// Ключ для хранения региона в свойствах объекта
const std::string UpdateRegionCommand::REGION_KEY = "CurrentRegion";

UpdateRegionCommand::UpdateRegionCommand(
    std::shared_ptr<IGameObject> object,
    std::shared_ptr<IRegionManager> regionManager,
    std::shared_ptr<ICollisionChecker> collisionChecker,
    std::shared_ptr<MacroCommand>& collisionMacro)
    : object_(std::move(object))
    , regionManager_(std::move(regionManager))
    , collisionChecker_(std::move(collisionChecker))
    , collisionMacro_(collisionMacro)
{
    if (!object_) {
        throw std::invalid_argument("Object cannot be null");
    }
    if (!regionManager_) {
        throw std::invalid_argument("RegionManager cannot be null");
    }
    if (!collisionChecker_) {
        throw std::invalid_argument("CollisionChecker cannot be null");
    }
}

void UpdateRegionCommand::execute() {
    // 1. Определяем новый регион на основе текущей позиции
    RegionId newRegion = regionManager_->getResolver().getRegion(*object_);
    
    // 2. Проверяем, был ли объект уже добавлен в систему
    if (hasStoredRegion()) {
        RegionId oldRegion = getStoredRegion();
        
        // 3. Если регион изменился - перемещаем объект
        if (newRegion != oldRegion) {
            regionManager_->moveObject(object_, oldRegion, newRegion);
            storeRegion(newRegion);
        }
    } else {
        // Первое добавление объекта в систему
        regionManager_->addObject(object_);
        storeRegion(newRegion);
    }
    
    // 4. Получаем всех соседей в текущем регионе
    auto neighbors = regionManager_->getObjectsInRegion(newRegion);
    
    // 5. Создаём команды проверки коллизий с каждым соседом
    std::vector<std::shared_ptr<ICommand>> collisionCommands;
    
    for (const auto& neighbor : neighbors) {
        // Пропускаем самого себя
        if (neighbor.get() == object_.get()) {
            continue;
        }
        
        // Создаём команду проверки коллизии
        collisionCommands.push_back(
            std::make_shared<CheckCollisionCommand>(
                object_, neighbor, collisionChecker_
            )
        );
    }
    
    // 6. Обновляем макрокоманду
    if (!collisionCommands.empty()) {
        collisionMacro_ = std::make_shared<MacroCommand>(collisionCommands);
    } else {
        collisionMacro_.reset();  // Нет соседей - нет команд
    }
}

RegionId UpdateRegionCommand::getStoredRegion() const {
    try {
        auto regionAny = object_->getProperty(REGION_KEY);
        return std::any_cast<RegionId>(regionAny);
    } catch (...) {
        return RegionId(0, 0);  // Значение по умолчанию
    }
}

void UpdateRegionCommand::storeRegion(const RegionId& region) {
    object_->setProperty(REGION_KEY, region);
}

bool UpdateRegionCommand::hasStoredRegion() const {
    try {
        object_->getProperty(REGION_KEY);
        return true;
    } catch (...) {
        return false;
    }
}