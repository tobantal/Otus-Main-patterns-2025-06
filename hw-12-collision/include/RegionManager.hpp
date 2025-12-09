#pragma once

#include "IRegionManager.hpp"
#include <unordered_map>
#include <unordered_set>

/**
 * @file RegionManager.hpp
 * @brief Реализация менеджера регионов
 * @author Anton Tobolkin
 */

/**
 * @brief Менеджер регионов
 * 
 * Хранит объекты, сгруппированные по регионам.
 * Использует хэш-таблицу для быстрого доступа к регионам.
 */
class RegionManager : public IRegionManager {
private:
    /// Resolver для определения региона по позиции
    std::shared_ptr<IRegionResolver> resolver_;
    
    /// Карта: регион -> множество объектов в нём
    std::unordered_map<RegionId, 
                       std::unordered_set<std::shared_ptr<IGameObject>>,
                       RegionIdHash> regions_;

public:
    /**
     * @brief Конструктор
     * @param resolver Resolver для определения регионов
     */
    explicit RegionManager(std::shared_ptr<IRegionResolver> resolver);

    void addObject(std::shared_ptr<IGameObject> object) override;
    void removeObject(std::shared_ptr<IGameObject> object) override;
    void moveObject(std::shared_ptr<IGameObject> object,
                   const RegionId& oldRegion,
                   const RegionId& newRegion) override;
    
    std::vector<std::shared_ptr<IGameObject>> 
        getObjectsInRegion(const RegionId& region) const override;
    
    const IRegionResolver& getResolver() const override;

    /**
     * @brief Получить количество непустых регионов
     */
    size_t getRegionCount() const;
};